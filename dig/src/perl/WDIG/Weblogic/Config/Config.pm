
# $Id$

=head1 NAME

WDIG::Weblogic::Config - Interface to BEA Weblogic configuration files.

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/04/23

=head1 SYNOPSIS

 use WDIG::Weblogic::Config;

 my $config = new WDIG::Weblogic::Config($myConfigFile);
 my $appConfig = $config->application($myAppName);
 my $serverConfig = $config->server($appConfig->{Target}[0]);
 my $port = $serverConfig->{ListenPort};

 $ENV{WL_HOME} = $wl_home;
 my @domains = WDIG::Weblogic::Config->getDomainList();

 my $config = new WDIG::Weblogic::Config($mydomain,
 					 wl_home=>$wl_home,
                                         use_defaults=>1,
                                         cache_file=>$myCache,
                                         xml_simple_opt=>\%myOpts);
 $config->getElement('JDBCConnectionPool',$myPoolName);

 
=head1 DESCRIPTION

The config module provides an interface to BEA Weblogic configuration files.
It currently only supports Weblogic 6 config.xml files.

=over 4

=item new($configFile || $domainName, %options)

Used to instantiate a new Config object.  This method will read the 
approprite config.xml file and construct a representative data structure
using XML::Simple.  If the first argument is a file name, that file is
used, otherwise the argument is used as the name of a Weblogic domain.

The wl_home option can be used to specify the Weblogic home - this should
contain the C<config> sub-directory. The WL_HOME environment variable
is used by default.

The use_defaults option is used to populate the default attibute values.
This is disabled by default.  Set the value to the location of an XML
Schema file for config.xml if you wish to use non-stadard defaults.
The XML::LibXSLT package is required for this option.

By default the config data structure is cached in 
C<$WL_HOME/config/$domain/config.cache>. The cache_file option can be
used to override this location.  Set to 0 to disable caching.

Use the xml_simple_opt to override the default options to XML::Simple,
C<ForceArray=E<gt>1, KeyAttr=E<gt>['Name']>.

=item application($applicationName)

Returns a data structure containing usefull detail on the configuration
of an application.  The data structure uses the following layout.

 $VAR1 = {
          'Target' => [
                        'myserver'
                      ],
          'Component' => {
                           'DefaultWebApp' => {
                                                'URI' => 'DefaultWebApp',
                                                'Type' => 'WebAppComponent'
                                              }
                         },
          'Path' => './config/mydomain/applications'
        };

If no application name is specified, a hash containing all applications
is returned.

=item server($serverName)

Returns a data structure containing usefull detail on the configuration
of a server.  The data structure uses the following layout.

 $VAR1 = {
          'Machine' => undef,
          'WebServer' => {
                           'FrontendHost' => undef,
                           'DefaultWebApp' => 'DefaultWebApp',
                           'LogFileName' => './config/mydomain/logs/access.log',
                           'FrontendHTTPPort' => undef
                         },
          'LogFileName' => 'config/mydomain/logs/weblogic.log',
          'ListenPort' => '7001'
        };

If no server name is specified, a hash containing all servers
is returned.

=item server2app($server_name,%options)

Returns a list of application names for those applications which
have a target as the specified server.  A reference to the list is
return in scalar context.

=item getElement($elementType,$elementName)

Returns the data structure, as constructed by XML::Simple, for the
appropriate element type and name.  The element type must be one of
the valid sub-elements of Domain, e.g. JDBCConnectionPool or Server.
The element name should match the value of the Name attribute of the
required element.  If no element name is specified then all elements 
of the specified type are returned.

=item getElementProperty($elementType,$elementName,$propertyName)

Returns the value of the specified property for the named configuration 
element.

=item getDomainList($wl_home)

Returns an array with the names of the domains found under the 
Weblogic config directory.  If wl_home is not specified then the
WL_HOME environment variable is used.

=back

=head1 ERROR HANDLING

Each method will croak() if the configuration file is not found or if
an invalid argument is specified.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Config.pm,v $
 Revision 1.3  2003/07/14 17:13:58  pegan
 Moved log & passwd methods off to the server class
 Changed storable to use net byte order

 Revision 1.2  2003/06/30 15:39:26  pegan
 Added server2app() and getElementProperty() methods
 Updated log_dir() search path

 Revision 1.1  2003/05/09 19:14:48  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Weblogic::Config;

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
use Sys::Hostname;
use File::Basename;
use File::Spec::Functions qw(catfile catdir updir no_upwards file_name_is_absolute);
use File::stat;
use Storable qw(nstore retrieve);
use XML::Simple qw(:strict);
use WDIG::File::Slurp;

my $Config_Dir = 'config';
my $Config_File_Name = 'config.xml';
my $Cache_File = 'config.cache';


######################################################################
# new($domain|$configFile,%options) - Config object constructor
# Returns new object
#

sub new
{
 my $self = shift();
 my $arg = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $config = {};

 bless($config, $package);

 $config->{wl_home} = $opt{wl_home} || $ENV{WL_HOME};

 $config->{opt} = \%opt;

 if ( defined($arg) && -f $arg ) {

   $config->{file} = $arg;
   $config->{wl_home} = $config->{wl_home} || catdir($arg,updir(),updir());

 } else {

   croak("WL_HOME is not defined!") if ( !$config->{wl_home} );
   croak("WL_HOME, $config->{wl_home}, not found!") if ( !-d $config->{wl_home} );
   croak("No config directory found in WL_HOME: $config->{wl_home}") if ( !-d catdir($config->{wl_home},$Config_Dir) );

   my @domains = $config->getDomainList();
   if ( defined($arg) && !grep(/$arg/,@domains) ) {
     croak("$arg is not a valid domain or config file!");
   }

   $config->{file} = catfile($config->{wl_home},$Config_Dir,$arg || $domains[0],$Config_File_Name);
   if ( !-r $config->{file} ) {
     croak("Default domain config not found in $config->{wl_home}!");
   }

 }

 if ( !defined($opt{cache_file}) || $opt{cache_file} ne "0" ) {
   $config->{cache_file} = $opt{cache_file} || catfile(dirname($config->{file}),$Cache_File);
 }
 
 $config->loadConfig();
 
 return ($config);
}


######################################################################
# getDomainList($wl_home) - reads config directory for list of
# domains. Returns an array of sub-directories found.
#

sub getDomainList
{
 my $config = shift();
 my $wl_home = shift() || $config->{wl_home} || $ENV{WL_HOME};

 my $config_dir = catfile($wl_home,$Config_Dir);

 croak("Config directory, $config_dir, not found!") if ( !-d $config_dir );

 opendir(DIR, $config_dir) || croak("$config_dir : $!");
 my @dirs = grep { -d catfile($config_dir,$_) } no_upwards(readdir(DIR));
 closedir DIR;

 my @domains = map { basename($_) } @dirs;

 return (@domains);
}


######################################################################
# loadConfig($file,%options) - Reads and parses the config file
# using XML::Simple.  Optionally applies the defaults XSLT.
# Returns 1 on success and 0 on failure
#

sub loadConfig
{
 my $config = shift();
 my $file = shift() || $config->{file};
 my %opt = @_;

 croak("Config file, $file, not found") if ( !-e $file );

 if ( defined($config->{cache_file}) && -e $config->{cache_file} &&
      stat($file)->mtime < stat($config->{cache_file})->mtime ) {
   $config->{data} = retrieve($config->{cache_file});
   carp("Error loading config cache") if ( !$config->{data}->{Domain} );
   $config->{name} = $config->{data}->{Name};
   return (1);
 }
 
 my $xml = readFile($file);
 croak("Config file, $file, is not a valid Weblogic config file") if ( !$xml );

 my $use_defaults = $opt{use_defaults} || $config->{opt}->{use_defaults};
 if ( $use_defaults ) {
   require WDIG::Weblogic::Config::Default;
   my $schema_file = ($use_defaults ne "1") ? $use_defaults : undef;
   $xml = WDIG::Weblogic::Config::Default::setDefaults($xml, schema_file=>$schema_file);
 }

 $config->{data}->{Domain} = {};

 my $xml_simple_opt = $opt{xml_simple_opt} || $config->{opt}->{xml_simple_opt};
 my $domain = {};
 croak("$file : $@") if ( !eval { $domain =
           XMLin($xml, ForceArray=>1, KeyAttr=>['Name'], KeepRoot=>1, %$xml_simple_opt)} );
 croak("Unable to parse $file") if ( !$domain->{Domain} or ref($domain->{Domain}) ne 'HASH' or keys(%{$domain->{Domain}}) != 1 );
 ($config->{name}, $config->{data}->{Domain}) = each(%{$domain->{Domain}});
 $config->{data}->{Name} = $config->{name};

 nstore($config->{data},$config->{cache_file}) if ($config->{cache_file});

 return (1);
}


######################################################################
# getElement($type,$name,%options) - Returns the configuration of
# the specified configuration element.
#

sub getElement
{
 my $config = shift();
 my $type = shift();
 my $name = shift();
 my %opt = @_;
 my $element;

 croak("No element type specified") if ( !defined($type) );
 croak("Element type '$type' not found") if ( !defined($config->{data}->{Domain}->{$type}) );

 my $elements = $config->{data}->{Domain}->{$type};

 if ($name) {
   croak("'$name' not found in domain '$config->{name}'") if ( !defined($elements->{$name}) );
   $element = $elements->{$name};
 }

 return ($element || $elements);
}


######################################################################
# getElementProperty($type,$name,$property,%options) - Returns 
# the value of the specified property for the named configuration element.
#

sub getElementProperty
{
 my $config = shift();
 my $type = shift();
 my $name = shift();
 my $property = shift();
 my %opt = @_;

 my $element = $config->getElement($type,$name,%opt);

 croak("'$property' not found in element '$name'") if ( !defined($element->{$property}) );

 return ($element->{$property});
}


######################################################################
# application($app_name,%options) - Returns a data structure with
# important detail on the application config.
#

sub application
{
 my $config = shift();
 my $app_name = shift();
 my %opt = @_;
 my $update = 0;

 my $apps = $config->{data}->{Domain}->{Application};

 croak("Application '$app_name' not found in domain '$config->{name}'")
                 if ( defined($app_name) && !defined($apps->{$app_name}) );

 foreach my $app ( $app_name || keys(%$apps) ) {
   if ( !defined($config->{data}->{Application}->{$app}) ){
     my $A = $config->{data}->{Application}->{$app} = {};
     $A->{Target} = [];
     $A->{Component} = {};
     $A->{Path} = $apps->{$app}->{Path};
     for my $type ( 'WebAppComponent', 'EJBComponent' ) {
       my $components = $apps->{$app}->{$type};
       foreach my $component ( keys(%$components) ) {
         my @targets = split(',', $components->{$component}->{Targets} || "");
         my %uniq; @uniq{(@{$A->{Target}}, @targets)}=();
         @{$A->{Target}} = keys(%uniq);
         $A->{Component}->{$component}->{Type} = $type;
         $A->{Component}->{$component}->{URI} = $components->{$component}->{URI};
       }
     }
     $update = 1;
   }
 }
 
 nstore($config->{data},$config->{cache_file}) if ($update and $config->{cache_file});

 return ($app_name ? $config->{data}->{Application}->{$app_name} : $config->{data}->{Application});
}


######################################################################
# server($server_name,%options) - Returns a data structure with
# important detail on the server config.
#

sub server
{
 my $config = shift();
 my $server_name = shift();
 my %opt = @_;
 my $update = 0;

 my $servers = $config->{data}->{Domain}->{Server};

 croak("Server '$server_name' not found in domain '$config->{name}'")
               if ( defined($server_name) && !defined($servers->{$server_name}) );

 foreach my $server ( $server_name || keys(%$servers) ) {
   if ( !defined($config->{data}->{Server}->{$server}) ){
     my $S = $config->{data}->{Server}->{$server} = {};
     $S->{Machine} = $servers->{$server}->{Machine};
     $S->{ListenPort} = $servers->{$server}->{ListenPort};
     foreach my $web_server ( values(%{$servers->{$server}->{WebServer}}) ){
       foreach my $attrib ( 'LogFileName', 'DefaultWebApp', 'FrontendHost', 'FrontendHTTPPort' ) {
         $S->{WebServer}->{$attrib} = $web_server->{$attrib};
       }
     }
     if ( ref($servers->{$server}->{Log}) eq 'HASH' ) {
       foreach my $log ( values(%{$servers->{$server}->{Log}}) ){
         $S->{LogFileName} = $log->{FileName};
       }
     } else { # if no Name attrib for Log element
       $S->{LogFileName} = $servers->{$server}->{Log}[0]->{FileName};
     }
     $update = 1;
  }
 }
 
 nstore($config->{data},$config->{cache_file}) if ($update and $config->{cache_file});

 return ($server_name ? $config->{data}->{Server}->{$server_name} : $config->{data}->{Server});
}


######################################################################
# server2app($server_name,%options) - Returns the list of applications
# targetted at the named server.
#

sub server2app
{
 my $config = shift();
 my $server_name = shift();
 my %opt = @_;

 my @app_list = ();

 my $apps = $config->application(undef,%opt);
 while ( my ($name, $app) = each(%$apps) ) {
   push(@app_list, $name) if ( grep { $_ eq $server_name } @{$app->{Target}} );
 }

 return (wantarray ? @app_list : \@app_list);
}


1;
__END__


