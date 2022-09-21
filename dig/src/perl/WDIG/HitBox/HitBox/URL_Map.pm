
# $Id$

=head1 NAME

WDIG::HitBox::URL_Map - Implements a mapping between URLs and certain values

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@disney.com
 Date: 2003/09/02

=head1 SYNOPSIS

 use WDIG::HitBox::URL_Map;

 my $map = new WDIG::HitBox::URL_Map();

 my $map = $hitbox_map->get_params($file);

=head1 DESCRIPTION

An interface to the HitBox url_map.xml file which maps files
to parameters such as counrty, unit & brand.  
The following methods are implemented:

=over 4

=item new($file,%options)

URL_Map object constructor takes the path to the map XML file
and a list of name=>value pair options.  If no file is specified
the default location is used.  This class works as a singleton,
i.e. the new method always returns the same object.
The valid options are:

cache_file - the location of file to store a cached copy of the
map.  Set to 0 to not use a cache file.

force_reload - force a reload of the data from disk rather than
using the in-memory copy.

xml_simple_opt - a hash reference to options for XML::Simple.
Defaults to C<ForceArray=E<gt>['path'],KeyAttr=E<gt>['root','pattern']>

=item get_params($file_name)

Returns a hash with the relevant parameters for the file.  The 
file name should be an absolute path.  The hash returned will 
contain the following keys:

 country - the country to which the file corresponds.
 unit - the business unit to which the file corresponds.
 brand - the brand to which the file corresponds.
 filename - the file name with any leading directory components removed.
 path - the leading directory components of the file name.
 path_less_pattern - the file name without the parts matched in the path pattern.
 doc_root - the document root used in matching the file.

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: URL_Map.pm,v $
 Revision 1.1  2003/09/03 06:06:09  pegan
 Initial revision - based on code from hitbox v2.3


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::HitBox::URL_Map;

use 5.006_001;
use strict;
use warnings;

our @ISA;
our $VERSION = '1.0';

######################################################################

use Carp;
use File::Basename;
use File::Spec::Functions qw(catfile catdir rel2abs);
use File::stat;
use Storable qw(retrieve nstore);
use XML::Simple qw(:strict);

my $URL_Map;

my $Default_Map_File = '/share/lib/hitbox/url_map.xml';
my $Cache_File = 'url_map.cache';

my $Default_Base = '/share/content';


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
 my $map = {};

 return ($URL_Map) if ( $URL_Map and !$opt{force_reload} );

 $map->{file} = $map_file || $Default_Map_File;
 $map->{mem_cache} = {};
 %{$map->{opt}} = %opt;

 bless($map, $package);

 if ( !defined($opt{cache_file}) or $opt{cache_file} ne "0" ) {
   $map->{cache_file} = $opt{cache_file} || catfile(dirname($map->{file}), $Cache_File);
 }

 $map->load();

 $URL_Map = $map;

 return ($map);
}


######################################################################
# load($file,%options) - Reads and parses the map file using XML::Simple.
# Returns 1 on success and 0 on failure
#

sub load
{
 my $map = shift();
 my $file = shift() || $map->{file};
 my %opt = @_;

 %opt = ( %{$map->{opt}}, %opt );

 if ( !-r $map->{file} ) {
   croak("Map file '$map->{file}' not found");
 }

 if ( defined($map->{cache_file}) and -r $map->{cache_file} and
      stat($file)->mtime < stat($map->{cache_file})->mtime ) {
   $map->{data} = retrieve($map->{cache_file});
   croak("Error loading map cache '$map->{cache_file}'") if ( ref($map->{data}) ne 'HASH' );
   return (1);
 }

 $map->{data} = {};

 my %xml_simple_opt = ( %{$opt{xml_simple_opt} || {}},
                        ForceArray => [ 'path' ],
			KeyAttr => [ 'root','pattern' ] );

 if ( !eval { $map->{data} = XMLin($file, %xml_simple_opt) } ) {
   croak("Could not parse the map file '$file': $@");
 }

 if ( !$map->{data}->{site} or ref($map->{data}->{site}) ne 'HASH' ) {
   croak("Could not parse the map file '$file'");
 }

 nstore($map->{data}, $map->{cache_file}) if ( $map->{cache_file} );

 return (1);
}



#########################################################################
# get_params($file) - determines the appropriate parameters for a file
# based on the mapping.
#

sub get_params
{
 my $map = shift();
 my $file = shift();
 my %opt = @_;

                                # Restore the results from the cache if found
 if ( !$opt{content_base} and $map->{mem_cache}->{$file} ) {
   my %copy = %{$map->{mem_cache}->{$file}};
   return (wantarray ? %copy : \%copy);
 }

 %opt = ( %{$map->{opt}}, %opt );

 my $base = $opt{doc_root_base} || $Default_Base || "";
 ( $base = rel2abs($base) ) =~ s/\/$//;

 my $doc_root;                  # Determine document root
 foreach my $root ( keys(%{$map->{data}->{site}}) ) {
   $doc_root = $root if ( $file =~ /^$base$root/ );
 }
 return(undef) if ( !$doc_root );

 my $match = {};                # Find matching path pattern with highest order:
 $match->{order} = -1;
 while ( my ($pat,$param) = each(%{$map->{data}->{site}->{$doc_root}->{path}}) ) {
   $pat =~ s/(?<!\.)\*/\/?(.*?)\/?/og; # DWIMify the pattern
   if ( $param->{order} > $match->{order} and
        ( ( my @sub_exp ) = ($file =~ /^$base$doc_root$pat$/) ) ) {
     $match = $param;
     $match->{path_less_pattern} = join('/', @sub_exp);
   }
 }

				# Add extra params
 $match->{filename} = (File::Spec::Unix->splitpath($file))[2];
 $match->{path} = (File::Spec::Unix->splitpath($file))[1];
 $match->{path_less_pattern} = (File::Spec::Unix->splitpath($match->{path_less_pattern}||""))[1];
 $match->{doc_root} = $base.$doc_root;

 %{$map->{mem_cache}->{$file}} = %{$match};

 return(wantarray ? %{$match} : $match);
}


1;
__END__

