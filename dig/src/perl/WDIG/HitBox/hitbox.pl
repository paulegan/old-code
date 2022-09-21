#!/usr/bin/perl -w

# $Id$

=head1 NAME

hitbox - Adds HitBox code to HTML & JSP files

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2001/05/01

=head1 SYNOPSIS

 hitbox [options] file ...
 hitbox -help

=head1 OPTIONS

  config - Specify an XML config file.  The elements should correspond
    to the names of these options.
  log_conf_file - The Log4perl configuration file.
  no_write - Does everything but write the HitBox code to the files.
  url_map_file - The location of the HitBox url_map.xml file.
  url_map_cache - The name of the cache file to use for url_map_file.
  hitbox_map_file - The location of the hitbox_map.xml.
  hitbox_map_cache - The name of the cache file to use for hitbox_map_file.
  tag_std - The location of the standard tag file.
  tag_flash - The location of the flash media tag file.
  insert_point - A regex specifying where to insert the tag by default.
  ref_db_file - The location of the reference database to use.
  ref_db_timeout - How long to wait before re-validating refs in the database.
  base - The document root base directory.
  file_match - A regex to specify which types of files to tag.
  quiet - Suppress informational messages.
  debug - Enable debug messages.
  help - Display this help message.

=head1 DESCRIPTION

The hitbox script is used to add the correct HitBox code to
content.  The code is determined by first using WDIG::HitBox::URL_Map
to map the file name to parameters such as country & business unit
and then mapping these values to the appropriate HitBox variables
using WDIG::HitBox::Param_Map.  These variables are combined with
a template tag and the result is inserted into the content file.

Certain files are excluded from tagging - framesets, non-content
frames, refresh pages and jsp & SSI includes.  In order to
accurately recognise frames & include files a database of references
from parent to child pages in maintained.

=head1 HISTORY

 $Log$
 Revision 3.0  2003/09/03 06:14:15  pegan
 Imported to CPAN style package with POD & makefile


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut

######################################################################

use 5.006_001;
use strict;
use warnings;
use bytes;

use lib('/share/src/perl/test_lib','/share/lib/perl5/5.6.1','/share/lib/perl5/site_perl');
use Getopt::Long;
use Pod::Usage;
use File::Spec::Functions qw(rel2abs);
use File::Spec::Unix;
use File::Find;
use XML::Simple;
use MLDBM qw(DB_File Storable);
use Log::Log4perl qw(:levels);
use WDIG::File::Slurp;
use WDIG::HitBox;
use WDIG::HitBox::URL_Map;
use WDIG::HitBox::Param_Map;


my @Opt_Config = (
  'defaults|config=s',
  'no_write',
  'log_conf_file=s',
  'url_map_file=s',
  'url_map_cache=s',
  'hitbox_map_file=s',
  'hitbox_map_cache=s',
  'tag_std=s',
  'tag_flash=s',
  'insert_point=s',
  'ref_db_file=s',
  'ref_db_timeout=s',
  'doc_root_base|base=s',
  'file_match=s',
  'debug',
  'quiet',
  'help|?'
);

my $Defaults_File = './config.xml';

my $Default_Log_Init = {
  'log4perl.rootLogger' => 'INFO, screen',
  'log4perl.appender.screen' => 'Log::Log4perl::Appender::Screen',
  'log4perl.appender.screen.stderr' => 0,
  'log4perl.appender.screen.layout' => 'SimpleLayout'
};

umask(0002);


######################################################################
# Process command line arguments:

my $config = {};
GetOptions($config, @Opt_Config) or pod2usage();
pod2usage(-exitstatus=>0, -verbose=>1) if ( $config->{help} );

pod2usage(-message=>"No file specified") if ( !@ARGV );

apply_defaults($config->{defaults} || $Defaults_File, $config);

Log::Log4perl->init($config->{log_conf_file} || $Default_Log_Init);
my $logger = Log::Log4perl->get_logger("");
$logger->level($FATAL) if ( $config->{quiet} );
$logger->level($DEBUG) if ( $config->{debug} );


######################################################################
# Initialization:


my $tag_std;						# Read the standard tag
if ( !-r $config->{tag_std} ) {
 $logger->logdie("Tag file '$config->{tag_std}': $!");
}
$tag_std = readFile($config->{tag_std});

my $tag_flash;						# Read the flash tag
if ( !-r $config->{tag_flash} ) {
 $logger->logdie("Tag file '$config->{tag_flash}': $!");
}
$tag_flash = readFile($config->{tag_flash});

my %ref_db;						# Open the ref DB
if ( ! tie(%ref_db, 'MLDBM', $config->{ref_db_file}) ) {
 $logger->logdie("Could not open the ref DB '$config->{ref_db_file}': $!");
}
tied(%ref_db)->DumpMeth('portable');

my $hitbox_map = new WDIG::HitBox::Param_Map($config->{hitbox_map_file},
                                       cache_file => $config->{hitbox_map_cache});

my $url_map = new WDIG::HitBox::URL_Map($config->{url_map_file},
                                       doc_root_base => $config->{doc_root_base},
                                       cache_file => $config->{url_map_cache});


######################################################################
# Main:

my @file_list;

foreach my $file ( @ARGV ) {				# Verify files & expand dirs
  $file = rel2abs($file);
  if ( !-e $file ) {
    $logger->error("$file: no such file or directory");
  } elsif ( -d $file ) {
    find(sub { push(@file_list,$File::Find::name) if (-f && ! -l && /$config->{file_match}/i); },$file);
  } else {
    push(@file_list,$file);
  }
}

FILE:
foreach my $file ( @file_list ) {			# For every file ...

  $file =~ s/\\/\//g;

  foreach my $pat ( @{$config->{filter}->{pattern}} ) {
    $pat =~ s/(?<!\.)\*/\/?(.*?)\/?/og;			# Skip filtered files
    if ( $file =~ /$pat/i ) {
      $logger->warn("$file: skipped; matched filter $pat");
      next FILE;
    }
  }

  my $contents = readFile($file);			# Read file contents

  if ( $contents =~ /$config->{refresh_page_pattern}/is ) { # Skip refresh pages
    $logger->warn("$file: skipped; contains refresh");
    next FILE;
  }

  my $file_params = $url_map->get_params($file);        # Get URL params
  if ( !$file_params ) {
    $logger->error("$file: No document root matches");
    next FILE;
  }
  if ( !$file_params->{country} ) {
    $logger->warn("$file: No parameters in map");
    next FILE;
  }
							# Get HitBox vars
  $file_params->{hitbox_vars} = $hitbox_map->get_vars($file_params);
  if ( !$file_params->{hitbox_vars} ) {
    $logger->error("$file:  No HitBox map found");
    next FILE;
  }
							# Parse include parent
  while ( $contents =~ /$config->{ssi_include_pattern}|$config->{jsp_include_pattern}/sogi ) {
    my $inc = $1 || $2;
    $inc = File::Spec::Unix->catfile($file_params->{($inc =~ /^\//)?'doc_root':'path'},$inc);
    mark_as_child(canonpath($inc),$file,'include');
  }

  my $is_frame_set = 0;					# Parse frameset
  while ( $contents =~ /$config->{frame_pattern}/sogi ) {
    my ($frame,$tag) = ($1,$&);
    if ( $& !~ /$config->{content_frame_pattern}/oi ) {
      $frame = File::Spec::Unix->catfile($file_params->{($frame =~ /^\//)?'doc_root':'path'},$frame);
      mark_as_child(canonpath($frame),$file,'frame');
    }
    $is_frame_set = 1;
  }

  if ( $is_frame_set ) {				# Skip framesets
    $logger->warn("$file: skipped; frameset");
    next FILE;
  }

  if ( my $parent = is_child($file) ) {			# Skip non-content frames & includes
    $logger->warn("$file: skipped; has parent $parent");
    remove_tag($file) unless ( $config->{no_write} );
    next FILE;
  }

							# Generate correct tag
  my $tag = ($contents =~ /$config->{flash_page_pattern}/soi)? $tag_flash : $tag_std;
  while ( my ($var,$value) = each(%{$file_params->{hitbox_vars}}) ) {
    $tag =~ s/\Q$var\E/$value/g;
  }
							# Insert tag
  if ( $contents =~ /$config->{tag_begin}.*$config->{tag_end}/so ) {
    if ($& eq $tag) { # Ignore if correct tag is already present
      $logger->info("$file: ".$file_params->{hitbox_vars}->{'CONTENT+CATEGORY'});
      next FILE;
    }
    $contents =~ s/$config->{tag_begin}.*$config->{tag_end}/$tag/so;
  } elsif ( $contents !~ /$config->{insert_point}/soi ) {
    $logger->error("$file: No insert point!");
    next FILE;
  } else {
    $contents =~ s/$config->{insert_point}/$tag$&/si;
  }
  
  unless ( $config->{no_write} ) {			# Write result
    if ( !open(F,">$file") ) {
      $logger->error("$file: Could not open for writing");
      next FILE;
    }
    print F $contents;
    close(F);
  }

  $logger->info("$file: ".$file_params->{hitbox_vars}->{'CONTENT+CATEGORY'});

}

untie(%ref_db);

exit(0);


#########################################################################
# mark_as_child(file,parent,type) - updates the ref DB to mark a file as
# a child and removes the HitBox code.
#

sub mark_as_child
{
 my $file = shift();
 my $parent = shift();
 my $type = shift();
 my $record = $ref_db{$file} || {};

 $record->{$parent} = {};
 $record->{$parent}->{lastUpdate} = time();
 $record->{$parent}->{type} = $type;

 $ref_db{$file} = $record;

 if ( !$config->{no_write} and remove_tag($file) ) {
   $logger->warn("$file: removed tag; has parent $parent");
 }

 return (1);
}


#########################################################################
# is_child(file) - returns true if the file is marked as a child in the
# ref DB.  If the data timeout has been reached the references are
# re-validated.
#

sub is_child
{
 my $file = shift();
 my $record = $ref_db{$file};
 my $now = time();
 my $ret;

 return(0) if ( !$record );

 foreach my $parent ( keys(%$record) ) {

   if (($now - $record->{$parent}->{lastUpdate}) > $config->{ref_db_timeout}) {

     my $contents = readFile($parent);
     my $file_params = $url_map->get_params($parent);

     if ( $record->{$parent}->{type} eq "include" ) {
       while ( $contents =~ /$config->{ssi_include_pattern}|$config->{jsp_include_pattern}/sogi ) {
         my $inc = $1 || $2;
         $inc = File::Spec::Unix->catfile($file_params->{($inc =~ /^\//)?'doc_root':'path'},$inc);
         $record->{$parent}->{lastUpdate} = $now if ( $file eq canonpath($inc) );
       }
     }

     if ($record->{$parent}->{type} eq "frame") {
       while ( $contents =~ /$config->{frame_pattern}/sogi ) {
         my ($frame,$tag) = ($1,$&);
         if ( $& !~ /$config->{content_frame_pattern}/oi ) {
           $frame = File::Spec::Unix->catfile($file_params->{($frame =~ /^\//)?'doc_root':'path'},$frame);
           $record->{$parent}->{lastUpdate} = $now if ( $file eq canonpath($frame) );
         }
       }
     }

     if ( $record->{$parent}->{lastUpdate} == $now ) {
       $ret = $ret ? "$parent and others" : $parent;
     } else {
       delete($record->{$parent});
     }

   } else { 

     my @parents = keys(%$record);
     $ret = $parents[0].($#parents?" and others":"");

   }
 }

 if ( !%$record ) {
   delete($ref_db{$file});
   return(0);
 } else {
   $ref_db{$file} = $record;
   return($ret);
 }
}


######################################################################
# apply_defaults($defaults,\%config) - Combines default config with
# the current values.
#

sub apply_defaults
{
 my $defaults = shift();
 my $config = shift();

 if ( ref($defaults) ne 'HASH' ) {

   die("Unable to parse defaults '$defaults': $@\n")
     if ( ! eval { $defaults = XMLin($defaults, ForceArray=>0, KeyAttr=>{}) } );

 }

 if ( $defaults and ref($defaults) eq 'HASH' ) {
   %$config = ( %$defaults, %$config );
   return (1);
 } else {
   return (undef);
 }
}


1;
__END__
