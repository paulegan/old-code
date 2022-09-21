
# $Id$

=head1 NAME

WDIG::Content::Drop - Object representation of a content drop

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/10

=head1 SYNOPSIS

 use WDIG::Content::Drop;

 my $drop1 = new WDIG::Content::Drop($src, $dst, @file_list);

 my $drop_xml = "$drop1";

 my $drop2 = new WDIG::Content::Drop($drop_xml);

 my $drop3 = $drop2->new();

=head1 DESCRIPTION

Object representation of a content drop.  The following methods
are implemented.

=over 4

=item new()

Creates a new Drop object.  If passed more than one argument 
C<new($source, $dest, @file_list)> is expected.  If only one 
argument is specified it must be either a reference to another
Drop object or an argument that XML::Simple will accept, such
as an XML string or a file name - e.g. C<new('drop.xml')>.
If called as an instance method no argument need be given
and the current object will be copied, C<$drop-E<gt>new()>.

=item as_xml()

Converts the drop to XML and returns that string.  Evaluation
of a Drop object in string context will call this method.

=item source($src)

Returns and optionally sets the drop source.

=item dest($dst)

Returns and optionally sets the drop destination.

=item file_list(@file_list)

Returns and optionally sets the drop file list.  

=item is_advanced_platform()

Returns true if this drop is destined for the advanced platform.

=item file_list_warn_handler(\%file_warnings,$warning)

A static function to be used to catch warnings from the file_list()
validation process and recover a hash with file names and 
warnings. e.g. -

 $SIG{__WARN__} = sub { return WDIG::Content::Drop::file_list_warn_handler(\%file_warnings,@_); };

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Drop.pm,v $
 Revision 1.5  2003/09/03 09:58:38  pegan
 Added file_list_warn_handler()

 Revision 1.4  2003/08/26 22:10:26  pegan
 added id() & project_id() methods

 Revision 1.3  2003/06/30 02:15:36  pegan
 Added do_validation flag to facilitate validation postponement

 Revision 1.2  2003/06/02 15:04:50  pegan
 Added dest URL verification with adv platform sub-class check
 Moved file name validation to untaint_file_name method

 Revision 1.1  2003/05/15 19:10:21  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Drop;

use 5.006_001;
use strict;
use warnings;

our @ISA = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use File::Spec::Functions qw(updir);
use URI;
use XML::Simple qw(:strict);
use WDIG::Content::URL_Map;

use overload '""' => 'as_xml';

our $Valid_Chars = '\w\\\/.:-';


######################################################################
# new($drop||$drop_xml) / new($src,$dst,@file_list) - 
# Drop object constructor; returns new object.
#

sub new
{
 my $self = shift();
 my $arg = shift();
 my $package = ref($self) || $self;

 $arg = $self if ( !$arg and ref($self) );

 my $drop = {};
 bless($drop, $package);

 # Validation is postponed until we determine the sub-class, if any.
 $drop->{do_validation} = undef;

 if ( @_ ) {
 
   $drop->source($arg);
   $drop->dest(shift());
   $drop->file_list(@_);
   
 } elsif ( UNIVERSAL::isa($arg, $package) ) {

   $drop->source($arg->source());
   $drop->dest($arg->dest());
   $drop->file_list($arg->file_list());
   $drop->project_id($arg->project_id());
   $drop->id($arg->id());

 } elsif ( $arg ) {
 
   my $data;

   croak("Unable to parse '$arg' as XML: $@")
     if ( ! eval { $data = XMLin($arg, ForceArray=>['file'], KeyAttr=>{}) } );

   $drop->source($data->{source}) or croak("No 'source' attribute found");
   $drop->dest($data->{dest}) or croak("No 'dest' attribute found");
   $drop->file_list($data->{file});
   $drop->project_id($data->{project_id});
   $drop->id($data->{id} || $data->{version});

 } else {

   croak('Usage: new($drop||$drop_xml) or new($src,$dst,@file_list)');

 }

 if ( $package !~ /Advanced/ and $drop->is_advanced_platform() ) {
   eval("require ${package}::Advanced");
   croak($@) if ( $@ );
   bless($drop, "${package}::Advanced");
 }

 # Now do the validation.  A bit inefficiently!
 $drop->{do_validation} = 1;
 $drop->source($drop->source());
 $drop->dest($drop->dest());
 $drop->file_list($drop->file_list());
 
 return ($drop);
}


######################################################################
# as_xml() - converts the data structure to XML and returns that string.
#

sub as_xml
{
 my $drop = shift();
 my $xml = undef;

 croak("Unable to convert to XML: $@")
   if ( ! eval { $xml = XMLout($drop->{data}, RootName=>"drop", KeyAttr=>{}) } );

 return ($xml);
}


######################################################################
# source($src) - Returns and optionally sets the drop source
#

sub source
{
 my $drop = shift();
 my $src = shift();

 return ($drop->{data}->{source}) if ( !$src and $drop->{data}->{source} );

 $src = new URI($src);

 if ( $drop->{do_validation} ) {

   croak("source must be a URL") if ( !$src );
   croak("source must be of file scheme") if ( !$src->scheme() or $src->scheme() ne 'file' );

   $drop->{source} = $drop->untaint_file_name($src->dir());

   croak("invalid source '$src'") if ( !$drop->{source} );
 
 }

 return ($drop->{data}->{source} = $src->as_string());
}

sub source_local
{
 my $drop = shift();
 return ($drop->{source});
}


######################################################################
# dest($dst) - Returns and optionally sets the drop destination
#

sub dest
{
 my $drop = shift();
 my $dst = shift();

 return ($drop->{data}->{dest}) if ( !$dst and $drop->{data}->{dest} );

 $dst = new URI($dst);

 if ( $drop->{do_validation} ) {

   if ( !$dst ) {

     croak("dest must be a URL");
   
   } elsif ( !$dst->scheme() or $dst->scheme() !~ /^(ftp|http|https)$/ ) {

     croak("dest must be of http, https or ftp scheme");

   } elsif ( !$dst->host() or !$drop->map()->contains($dst) ) {

     croak("dest host '".$dst->host()."' not found");

   }

 }

 return ($drop->{data}->{dest} = $dst->as_string());
}

sub dest_machine
{
 my $drop = shift();

 $drop->{dest}->{machine} = $drop->map()->to_machine($drop->dest()) if ( !$drop->{dest}->{machine} );

 return ($drop->{dest}->{machine});
}

sub dest_root
{
 my $drop = shift();

 $drop->{dest}->{root} = $drop->map()->to_path_root($drop->dest()) if ( !$drop->{dest}->{root} );

 return ($drop->{dest}->{root});
}


######################################################################
# file_list(@file_list) - Returns and optionally sets the drop file
# list.
#

sub file_list
{
 my $drop = shift();
 my @list = ref($_[0]) ? @{$_[0]} : @_;

 if ( !defined($_[0]) ) {
   return ( $drop->{data}->{file} ? @{$drop->{data}->{file}} : () );
 }
 
 if ( $drop->{do_validation} ) {

   $drop->{data}->{file} = [];

   foreach my $file ( @list ) {

     my $file_name = $drop->untaint_file_name($file, make_relative=>1);
     if ( !$file_name ) {
       carp("invalid file name '$file'");
       next;
     }

     push( @{$drop->{data}->{file}}, $file_name );

   }

 } else {

   @{$drop->{data}->{file}} = @list;

 }

 return (@{$drop->{data}->{file}});
}


######################################################################
# project_id($project_id) - Returns and optionally sets the drop
# project ID from Pluto
#

sub project_id
{
 my $drop = shift();
 my $project_id = shift();

 $drop->{data}->{project_id} = $project_id if ( $project_id );

 # Some very bad magic until pluto is changed to explicitly specify the project ID:
 if ( !$drop->{data}->{project_id} and $drop->source() =~ /repository\/(\d+)/ ) {
   $drop->{data}->{project_id} = $1;
 }

 return ($drop->{data}->{project_id});
}


######################################################################
# id($id) - Returns and optionally sets the drop ID/version
# as from Pluto
#

sub id
{
 my $drop = shift();
 my $id = shift();

 $drop->{data}->{id} = $id if ( $id );

 return ($drop->{data}->{id});
}

sub version { return id(@_) };


######################################################################
# map() - Gets the URL map
#

sub map()
{
 my $drop = shift();
 $drop->{url_map} = new WDIG::Content::URL_Map() if ( !$drop->{url_map} );
 return ($drop->{url_map})
}


######################################################################
# is_advanced_platform() - Returns true if this drop is destined
# for the advanced platform.
#

sub is_advanced_platform
{
 my $drop = shift();

 if ( !exists($drop->{adv}) and
       $drop->map()->is_advanced_platform($drop->dest()) ) {
   $drop->{adv} = {};
 }
 return ( exists($drop->{adv}) );
}


######################################################################
# untaint_file_name() - verifies that the file path does not contain
# a parent dir reference and that only valid characters are used.
# The laundered file name is returned or undef if it's invalid.
# 

sub untaint_file_name
{
 my $drop = shift();
 my $file_name = shift();
 my %opt = @_;

 return (undef) if ( index($file_name,updir())>0 );

 my $valid_RE = $opt{valid_chars} ? "([$opt{valid_chars}]+?)\/*" : "([$Valid_Chars]+?)\/*";
 $valid_RE = '\.*\/*'.$valid_RE if ( $opt{make_relative} );

 if ( $file_name !~ /^$valid_RE$/ ) {
   return (undef);
 } else {
   return ($1);
 }

}


######################################################################
# file_list_warn_handler(\%file_warnings,$warning) - a utility 
# function to be used to catch warnings from the file_list()
# validation process and recover a hash with file names and 
# warnings.
#

sub file_list_warn_handler
{
 my $file_warnings = shift() || {};
 my $warning = shift();

 # this pattern should match the text of the carp() call in file_list()
 $file_warnings->{$2} = $1 if ( $warning =~ /(.*) '([^']*)'/ );

 return ($file_warnings);
}

1;
__END__


