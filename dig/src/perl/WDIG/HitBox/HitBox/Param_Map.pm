
# $Id$

=head1 NAME

WDIG::HitBox::Param_Map - Implements a mapping between file params and HitBox vars

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@disney.com
 Date: 2003/09/02

=head1 SYNOPSIS

 use WDIG::HitBox::Param_Map;

 my $hitbox_map = new WDIG::HitBox::Param_Map();

 my $hitbox_vars = $hitbox_map->get_vars(
                                  country => $country,
                                  unit => $unit,
                                  brand => $brand
                                 );

=head1 DESCRIPTION

An interface to the hitbox_map.xml file which maps file
parameters (usually obtained from url_map.xml) to HitBox
variables.  The following methods are implemented:

=over 4

=item new($file,%options)

Param_Map object constructor takes the path to the map XML file
and a list of name=>value pair options.  If no file is specified
the default location is used.  This class works as a singleton,
i.e. the new method always returns the same object. 
The valid options are:

cache_file - the location of file to store a cached copy of the
map.  Set to 0 to not use a cache file.

force_reload - force a reload of the data from disk rather than
using the in-memory copy.

xml_simple_opt - a hash reference to options for XML::Simple.
Defaults to C<ForceArray=E<gt>['unit','map'],KeyAttr=E<gt>{}>

=item get_vars(%params)

Returns a hash with the HitBox variables for the given parameters.
The input should contain the keys C<country>, C<unit> & C<brand>
at minimum.  The hash returned will contain the following keys:

 ACCOUNT+ID+LIST
 CONTENT+CATEGORY
 PUT+PAGE+NAME+HERE
 ENTRY+CAMPAIGN+ID
 GOAL+CAMPAIGN+ID
 SEGMENT+ID

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Param_Map.pm,v $
 Revision 1.1  2003/09/03 06:15:40  pegan
 Initial revision - based on code from hitbox v2.3


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::HitBox::Param_Map;

use 5.006_001;
use strict;
use warnings;

our @ISA;
our $VERSION = '1.0';

######################################################################

use Carp;
use File::Basename;
use File::Spec::Functions qw(catfile catdir);
use File::stat;
use Storable qw(retrieve nstore);
use XML::Simple qw(:strict);

my $Param_Map;

my $Default_Map_File = '/share/lib/hitbox/hitbox_map.xml';
my $Cache_File = 'hitbox_map.cache';


######################################################################
# new($mapFile,%options) - Param_Map object constructor
# Returns new object
#

sub new
{
 my $self = shift();
 my $map_file = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $map = {};

 return ($Param_Map) if ( $Param_Map and !$opt{force_reload} );

 $map->{file} = $map_file || $Default_Map_File;
 %{$map->{opt}} = %opt;

 bless($map, $package);

 if ( !defined($opt{cache_file}) or $opt{cache_file} ne "0" ) {
   $map->{cache_file} = $opt{cache_file} || catfile(dirname($map->{file}), $Cache_File);
 }

 $map->load();

 $Param_Map = $map;

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
                        ForceArray => [ 'unit', 'map' ],
			KeyAttr => {} );

 if ( !eval { $map->{data} = XMLin($file, %xml_simple_opt) } ) {
   croak("Could not parse the map file '$file': $@");
 }

 if ( !$map->{data}->{unit} or ref($map->{data}->{unit}) ne 'ARRAY' ) {
   croak("Could not parse the map file '$file'");
 }

 nstore($map->{data}, $map->{cache_file}) if ( $map->{cache_file} );

 return (1);
}



#########################################################################
# get_vars(%params) - returns a hash with HitBox variables.
#

sub get_vars
{
 my $self = shift();

 my $params = ref($_[0]) eq 'HASH' ? shift() : { @_ };

 my $map = undef;		# Find matching HitBox map
 foreach my $unit ( @{$self->{data}->{unit}} ) {
   if ( $unit->{country_code} eq $params->{country} and
        $unit->{unit_code} eq $params->{unit} and
        $unit->{brand_code} eq $params->{brand} ) {
     $map = $unit->{map};
     last;
   }
 }

 return (undef) if ( !$map );

 my ( @cat,@acct );		# Combine the resulting categories & accounts
 foreach my $m ( @{$map} ) {
   push(@acct, $m->{account});
   push(@cat, catdir($m->{category},
              $params->{site_name} || "",
              $params->{path_less_pattern} || ""));
 }

 my $vars = {};
 $vars->{'ACCOUNT+ID+LIST'} = join(";",@acct);
 $vars->{'CONTENT+CATEGORY'} = join(";",@cat);
 $vars->{'PUT+PAGE+NAME+HERE'} = $params->{filename} || "unknown";
 $vars->{'ENTRY+CAMPAIGN+ID'} = "";
 $vars->{'GOAL+CAMPAIGN+ID'} = "";
 $vars->{'SEGMENT+ID'} = "";

 return(wantarray ? %{$vars} : $vars);
}


1;
__END__


