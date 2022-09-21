
# $Id$

=head1 NAME

WDIG::Content::URL_Map - Implements a mapping between URLs and certain values

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.egan@disney.com
 Date: 2003/05/15

=head1 SYNOPSIS

 use WDIG::Content::URL_Map;

 my $map = new URL_Map();

 my $variable = $map->to_param($url, $var_name);

 my $machine = $map->to_machine($url) if ( $map->contains($url) );

=head1 DESCRIPTION

This module uses an XML file to map URLs to certain values.  A
URL can be a string such as "http://disney.ie" or a URI object.
The following methods are implemented:

=over 4

=item new($file,%options)

URL_Map object constructor takes the path to the map XML file
and a list of name=>value pair options.  The valid options are:

cache_file - the location of file to store a cached copy of the
map.  Set to 0 to not use a cache file.

xml_simple_opt - a hash reference to options for XML::Simple.
Defaults to C<ForceArray=E<gt>['param'],KeyAttr=E<gt>{param=E<gt>'name'},ForceContent=E<gt>1>

=item contains($url)

Returns true if the specified URL exists in the map

=item to_host($url)

Returns the host for the specified URL

=item to_protocol($url)

Returns the protocol for the specified URL

=item to_path_root($url)

Returns the root path for the specified URL

=item to_machine($url)

Returns the machine name for the specified URL

=item to_machine_type($url)

Returns the machine type for the specified URL

=item is_live_url($url)

Returns true if the specified URL is live

=item is_staging_url($url)

Returns true if the specified URL is staging

=item is_dev_url($url)

Returns true if the specified URL is dev

=item is_advanced_platform($url)

Returns true if the platform type is advanced.

=item is_standard_platform($url)

Returns true if the platform type is standard.

=item to_param($url,$name)

Returns the named parameter for the specified URL

=item search(%search)

Returns the list of URLs that match all search criteria.
The search arguments should be in the form of attribute/value 
pairs, e.g. C<search(role=E<gt>'live', machine=E<gt>'my_host')>

=back

=head1 NOTES

This class will really need to be re-worked later on to include
support for path mappings.

XML::Simple is not the best for this job as it doesn't create
a suitable data structure from the schema.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: URL_Map.pm,v $
 Revision 1.5  2003/08/29 03:05:50  pegan
 Import nstore from Storable

 Revision 1.4  2003/08/26 22:07:44  pegan
 Fixed small bug with is_advanced_platform()

 Revision 1.3  2003/07/14 16:31:32  pegan
 Changed storable to net byte order

 Revision 1.2  2003/06/30 02:22:15  pegan
 Changed accessor method names
 Added create_key(), search() and map_file() methods

 Revision 1.1  2003/05/15 01:52:51  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::URL_Map;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw();
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use File::Basename;
use File::Spec::Functions qw(catfile catdir);
use File::stat;
use URI;
use Storable qw(retrieve nstore);
use XML::Simple qw(:strict);

my $URL_Map;

my $Default_Map_File = '/share/lib/url_map.xml';
my $Cache_File = 'url_map.cache';

my $Key_Sep = '://';

my $Type_Standard = 'standard';
my $Type_Advanced = 'advanced';

my $Role_Live = 'live';
my $Role_Staging = 'staging';
my $Role_Dev = 'dev';

my $Default_Machine_Type = 'sun';


######################################################################
# new($mapFile,%options) - URL_Map object constructor
# Returns new object
#

sub new
{
 my $self = shift();
 my $map_file = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $url_map = {};

 return ($URL_Map) if ( $URL_Map and !$opt{force_reload} );
 
 $url_map->{file} = $map_file || $Default_Map_File;
 %{$url_map->{opt}} = %opt;

 bless($url_map, $package);

 if ( !defined($opt{cache_file}) or $opt{cache_file} ne "0" ) {
   $url_map->{cache_file} = $opt{cache_file} || catfile(dirname($url_map->{file}), $Cache_File);
 }

 $url_map->load();

 $URL_Map = $url_map;
 
 return ($url_map);
}


######################################################################
# load($file,%options) - Reads and parses the map file using XML::Simple.
# Returns 1 on success and 0 on failure
#

sub load
{
 my $url_map = shift();
 my $file = shift() || $url_map->{file};
 my %opt = @_;

 %opt = ( %{$url_map->{opt}}, %opt );

 if ( !-r $url_map->{file} ) {
   croak("Map file $url_map->{file} not found");
 }

 if ( defined($url_map->{cache_file}) and -r $url_map->{cache_file} and
      stat($file)->mtime < stat($url_map->{cache_file})->mtime ) {
   $url_map->{data} = retrieve($url_map->{cache_file});
   croak("Error loading map cache") if ( ref($url_map->{data}) ne 'HASH' );
   return (1);
 }

 my $map = $url_map->{data} = {};

 my %xml_simple_opt = ( %{$opt{xml_simple_opt} || {}}, 
                        ForceArray => [ 'url', 'param' ], 
			KeyAttr => { param=>'name' },
			ForceContent => 1 );

 if ( !eval { $map = XMLin($file, %xml_simple_opt) } ) {
   croak("$file : $@");
 }

 if ( !$map->{url} or ref($map->{url}) ne 'ARRAY' ) {
   croak("Unable to parse $file");
 }
 
 foreach my $url ( @{$map->{url}} ) {
   my $key = join($Key_Sep, $url->{protocol}, $url->{host});
   $url_map->{data}->{$key} = $url;
 }

 nstore($url_map->{data}, $url_map->{cache_file}) if ($url_map->{cache_file});

 return (1);
}


######################################################################
# create_key($uri) - creates a hash key from the specified URL
#

sub create_key
{
 my $url_map = shift();
 my $url = shift();

 $url = new URI($url) or croak("Could not convert $url to URI");

 croak("URL must contain scheme & host elements") if ( !$url->scheme() or !$url->host() );

 return (join($Key_Sep, $url->scheme(), $url->host()));
}


######################################################################
# contains($url) - returns true is the specified URL exists in the map
#

sub contains
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key});
}


######################################################################
# to_host($url) - returns the host for the specified URL
#

sub to_host
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key}->{host});
}


######################################################################
# to_protocol($url) - returns the protocol for the specified URL
#

sub to_protocol
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key}->{protocol});
}


######################################################################
# to_path_root($url) - returns the root for the specified URL
#

sub to_path_root
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key}->{root}->{content});
}


######################################################################
# to_machine($url) - returns the machine for the specified URL
#

sub to_machine
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key}->{machine}->{content});
}


######################################################################
# to_machine_type($url) - returns the machine type for the specified URL
#

sub to_machine_type
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key}->{machine}->{type} || $Default_Machine_Type);
}


######################################################################
# is_live_url($url) - returns true if the specified URL is live
#

sub is_live_url
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return (lc($url_map->{data}->{$key}->{role}->{content}) eq $Role_Live);
}


######################################################################
# is_staging_url($url) - returns true if the specified URL is staging
#

sub is_staging_url
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return (lc($url_map->{data}->{$key}->{role}->{content}) eq $Role_Staging);
}


######################################################################
# is_dev_url($url) - returns true if the specified URL is dev
#

sub is_dev_url
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return (lc($url_map->{data}->{$key}->{role}->{content}) eq $Role_Dev);
}


######################################################################
# is_advanced_platform($url) - returns true if the platform type is
# advanced.
#

sub is_advanced_platform
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return ($url_map->{data}->{$key} and lc($url_map->{data}->{$key}->{platform}->{content}) eq $Type_Advanced);
}


######################################################################
# is_standard_platform($url) - returns true if the platform type is
# standard.
#

sub is_standard_platform
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());

 return (lc($url_map->{data}->{$key}->{platform}->{content}) eq $Type_Standard);
}


######################################################################
# to_param($url,$name) - returns the named parameter for the specified URL
#

sub to_param
{
 my $url_map = shift();
 my $key = $url_map->create_key(shift());
 my $name = shift();

 return ($url_map->{data}->{$key}->{param}->{$name}->{content});
}


######################################################################
# search(%search) - returns the named parameter for the specified URL
#

sub search
{
 my $url_map = shift();
 my %search = @_;

 my @keys = ();

 while ( my ($key, $url) = each(%{$url_map->{data}}) ) {
   my $match = 0;
   while ( my ($attrib, $value) = each(%search) ) {
     if ( $url->{$attrib} ) {
       if ( $url->{$attrib} eq $value or
          ( ref($url->{$attrib}) and $url->{$attrib}->{content} eq $value ) ) {
         $match++;
       }
     } else {
       if ( $url->{param} and $url->{param}->{$attrib} and
            $url->{param}->{$attrib}->{content} eq $value ) {
         $match++;
       }
     }
   }
   push(@keys, $key) if ( $match == @_/2 );
 }

 return (@keys);
}


######################################################################
# map_file() - Returns the name of the map file used.
#

sub map_file
{
 my $url_map = shift();
 return ($url_map->{file});
}

1;
__END__


