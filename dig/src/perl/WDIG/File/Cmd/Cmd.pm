
# $Id$

=head1 NAME

WDIG::File::Cmd - Collection of utility functions for dealing with files.

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.egan@disney.com
 Date: 2001/11/26

=head1 SYNOPSIS

 use WDIG::File::Cmd;
 
  md('/my/new/dir', recurse=>1);
  cp('/etc/passwd', '/my/new/dir', preserve_mode=>1);
  mv('passwd', 'passwd2', wd=>'/my/new/dir');
  rm('/my/new/dir', recurse=>1);

=head1 DESCRIPTION

The cmd module implements some standard file management functions.  These
work in a similar manner to the Unix commands cp, mv, mkdir & rm.  The first
argument of each is a file or list of files (as an array reference).  Each
function accepts a list of name=>value option pairs for modifying its
behaviour.  The C<wd> options can be used with each function wo change
the working directory.

=head2 rm()

The rm function will remove each file or directory specified.
Expects a file name or reference to an array of file names, followed
by options.

 rm($file, %options);
 rm(\@files, option=>$value);

Options supported:

=over 4

=item wd

Specify a directory to change to before executing operation.  Relative
path names will use this base.

=item force

if true, remove file even when parent directory is write protected.

=item recurse

As "rm -r", if true, recursively remove files and subdirectories.

=back

=head2 md()

The md function creates the specified directories.
Expects a dir name or reference to an array of dir names, followed
by options.

 md($dir, %options);
 md(\@dirs, option=>$value);

Options supported:

=over 4

=item wd

Specify a directory to change to before executing operation.  Relative
path names will use this base.

=item recurse

As "mkdir -p", if true, creates parent directories if they don't exist.

=item mode

Specify the file mode to create the directory with.  Defaults to 0777.

=item user group

If either the user or group options are specified, then the user or
group for the directory is set to these.  Must be specified as the ID
number and not the text name.

=back

=head2 mv()

The mv function moves the specified file or directories to a new location.
Expects a file name or reference to an array of file names, followed
by the destination and then options.  The destination argument must be
a directory if the source is an array reference.

 mv($dir, $new_dir, %options);
 mv(\@files, $dir, option=>$value);

Options supported:

=over 4

=item wd

Specify a directory to change to before executing operation.  Relative
path names will use this base.

=item preserve_path

Preserves the path component of the source, e.g.
C<mv('my/file', 'my/dir', preserve_path=E<gt>1)> will move the file to
C<my/dir/my/file> rather than the usual C<my/dir/file>.

=item mode

Specify the file mode to create the directory with.  Defaults to 0777.

=item user group

If either the user or group options are specified, then the user or
group for the directory is set to these.  Must be specified as the ID
number and not the text name.

=back

=head2 cp()

The cp function copies the specified files to a new location.
Expects a file name or reference to an array of file names, followed
by the destination and then options.  The destination argument must be
a directory if the source is an array reference.

 cp($file, $new_file, %options);
 mv(\@files, $dir, option=>$value);

Options supported:

=over 4

=item wd

Specify a directory to change to before executing operation.  Relative
path names will use this base.

=item preserve_mode

If true (the default), then the file mode of the destination will be set
to the same as the source.

=item preserve_path

Preserves the path component of the source, e.g.
C<cp('my/file', 'my/dir', preserve_path=E<gt>1)> will copy the file to
C<my/dir/my/file> rather than the usual C<my/dir/file>.

=item mode

Specify the file mode to create the directory with.  Defaults to 0777.

=item user group

If either the user or group options are specified, then the user or
group for the directory is set to these.  Must be specified as the ID
number and not the text name.

=back

=head1 ERROR HANDLING

Each function will return the value 1 if the operation completed 
sucessfully.  On error the value 0 will be returned.  The C "errno" 
variable, $!, should be checked for further information on the cause.

When a file list is specified, any error with one of the files will
cause the value 0 to be returned but the remaining files in the list
will still be processed.

Currently failure to set file modes or user & group ownership does
not result in an error.

=head1 EXPORTS

 cp
 mv
 rm
 md
 
=head1 HISTORY

 $Log: Cmd.pm,v $
 Revision 1.4  2003/04/21 18:20:17  pegan
 Rewritten to use array refs and name=>value option pairs
 Wrote POD

 Revision 1.3  2003/04/17 16:02:24  pegan
 Transformed FileUtil.pm to WDIG::File::Cmd
 Added POD

 Revision 1.2  2003/04/17 15:35:01  root
 Updated GID
 Added to bin ext list

 Revision 1.1  2002/01/29 02:49:00  pegan
 Initial revision

=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::File::Cmd;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(cp mv rm md);
our @EXPORT_OK = qw();
our $VERSION = '1.0';


######################################################################

use bytes;
use Carp;
use File::Copy;
use File::Basename;
use File::Spec::Functions;
use File::stat;
use Cwd;

my %DefaultOpt = (
  wd=>0,
  force=>0,
  recurse=>0,
  preserve_mode=>1,
  preserve_path=>0,
  mode=>0, 
  user=>-1, 
  group=>-1
);

			
######################################################################
# getOpt(%options) - Validates name=>value pair command options
# Returns a hash ref
#

sub getOpt
{
 croak("Options must be name=>value pairs") if (@_ % 2);
 
 my $opt = {};
 my %new = @_;
 
 %$opt = %DefaultOpt;
 while ( my($key,$val) = each(%new) ) {
  croak("Unrecognised option: $key") unless(exists($opt->{$key}));
  $opt->{$key} = $val;
 }
 
 return ($opt);
}


######################################################################
# rm($file,%options) - Removes a file or directory
# Returns 1 on success and 0 on failure
#

sub rm
{
 my $file = shift();
 my $opt = getOpt(@_);
 my $success = 1;
 my $wd = cwd();
 my @files = ( $file );

 if ( $opt->{wd} ) {
   chdir($opt->{wd}) || return (0);
 }

 if ( ref($file) && ref($file) eq 'ARRAY' ) {
   @files = @$file;
 }
 
 foreach my $file ( @files ) {
 
   chmod(0777,$file) if ( $opt->{force} );
 
   if ( -d $file ) {
 
     if ( $opt->{recurse} ){
       if ( opendir(DIR,$file) ) {
   	 my @files = no_upwards(readdir(DIR));
   	 closedir(DIR);
   	 my %opt = %$opt; $opt{wd} = 0;
   	 for ( @files ) { /^(.+)$/; rm(catfile($file,$1),%opt); }
       } else {
   	 $success = 0;
       }
     }
     $success = 0 if ( !rmdir($file) );
     
   } else {
 
     $success = 0 if ( !unlink($file) );

   }
 
 }
 
 chdir($wd) if ( $opt->{wd} );

 return ($success);
}


######################################################################
# md($dir,%options) - creates a directory
# Returns 1 on success and 0 on failure
#

sub md
{
 my $dir = shift();
 my $opt = getOpt(@_);
 my $success = 1;
 my $wd = cwd();
 my @dirs = ( $dir );

 if ( $opt->{wd} ) {
   chdir($opt->{wd}) || return (0);
 }
 
 if ( ref($dir) && ref($dir) eq 'ARRAY' ) {
   @dirs = @$dir;
 }
 
 foreach my $dir ( @dirs ) {
 
   if ( $opt->{recurse} ) {
     my $parent = dirname($dir);
     my %opt = %$opt; $opt{wd} = 0;
     md($parent,%opt) if ( !-e $parent );
   }
 
   if ( !mkdir($dir,$opt->{mode} || 0777) ) {
     $success = 0;
     next;
   }

   if ($opt->{user}+$opt->{group}!=-2) {
     chown($opt->{user}, $opt->{group}, $dir);
   }
   
 }
 
 chdir($wd) if ( $opt->{wd} );

 return ($success);
}


######################################################################
# mv($source,$dest,%options) - moves a file or files
# Returns 1 on success and 0 on failure
#

sub mv
{
 my $source = shift();
 my $dest = shift();
 my $opt = getOpt(@_);
 my $success = 1;
 my $wd = cwd();
 my @files = ( $source );
 
 if ( $opt->{wd} ) {
   chdir($opt->{wd}) || return (0);
 }
  
 #chmod(0777,$dest) if ( $opt->{force} );

 if ( ref($source) && ref($source) eq 'ARRAY' ) {
   if ( !-d $dest ) {
     $! = 20;	# dest must be a dir when using multiple sources
     chdir($wd); return (0);
   }
   @files = @$source;
 }
 
 foreach my $file ( @files ) {

   my $file_dest = $dest;
   if (-d $dest) {
     if ( $opt->{preserve_path} ) {
       $file_dest = catfile($dest,$file);
       md(dirname($file_dest),recurse=>1) if ( !-e dirname($file_dest) );
     } else {
       $file_dest = catfile($dest,basename($file));
     }
   }

   if ( !move($file,$file_dest) ) {
     $success = 0;
     next;
   }

   if ($opt->{mode}) {
     chmod($opt->{mode}, $file_dest);
   }
   if ($opt->{user}+$opt->{group}!=-2) {
     chown($opt->{user}, $opt->{group}, $file_dest);
   }

 }

 chdir($wd) if ( $opt->{wd} );

 return ($success);
}


######################################################################
# cp($source,$dest,%options) - Copies files to a new location
# Returns 1 on success and 0 on failure
#

sub cp
{
 my $source = shift();
 my $dest = shift();
 my $opt = getOpt(@_);
 my $success = 1;
 my $wd = cwd();
 my @files = ( $source );
 
 if ( $opt->{wd} ) {
   chdir($opt->{wd}) || return (0);
 }

 if ( ref($source) && ref($source) eq 'ARRAY' ) {
   if ( !-d $dest ) {
     $! = 20;	# dest must be a dir when using multiple sources
     chdir($wd); return (0);
   }
   @files = @$source;
 }
 
 foreach my $file ( @files ) {

   my $file_dest = $dest;
   if (-d $dest) {
     if ( $opt->{preserve_path} ) {
       $file_dest = catfile($dest,$file);
       md(dirname($file_dest),recurse=>1) if ( !-e dirname($file_dest) );
     } else {
       $file_dest = catfile($dest,basename($file));
     }
   }

   rm($file_dest) if ( $opt->{force} && -e $file_dest && !-w _ );

   if ( !copy($file,$file_dest) ) {
     $success = 0;
     next;
   }

   if ( $opt->{preserve_mode} ) {
     chmod(stat($file)->mode & 0777, $file_dest);
   }
   if ($opt->{mode}) {
     chmod($opt->{mode}, $file_dest);
   }
   if ($opt->{user}+$opt->{group}!=-2) {
     chown($opt->{user}, $opt->{group}, $file_dest);
   }

 }

 chdir($wd) if ( $opt->{wd} );

 return ($success);
}


1;
__END__

