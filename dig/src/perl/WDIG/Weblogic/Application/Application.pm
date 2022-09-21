
# $Id$

=head1 NAME

WDIG::Weblogic::Application - OO interface for the management of a Weblogic application

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.egan@disney.com
 Date: 2003/05/07

=head1 SYNOPSIS

 use WDIG::Weblogic::Application;

 $app = new WDIG::Weblogic::Application($appName, $domainName);

 my @servers = $app->targets();

 $app->start();

 $app->stop() if ( $app->ping() );

=head1 DESCRIPTION

This class implements an OO interface for the management of a 
Weblogic applications.  Configuration information is read using 
WDIG::Weblogic::Config from which a list of targeted servers
is retrieved.  Each operation requested on the application is
passed to each of the servers using WDIG::Weblogic::Server.
The following methods are available.

=over 4

=item new($name,$domain,%opt)

The constructor creates a new application object based on the configuration
found in the specified domain.  The following options are supported by
both the constructor and individual methods.  Options to the methods
will override those of the constructor.

 defaults - A hash reference or an object suitable for
   XML::Simple (XML doc or file) that will be used to
   specify default options.  The XML::Simple options
   used are "ForceArray=>0, KeyAttr=>{}".

 wl_home - Specifies the directory where the Weblogic
   config sub-dir can be found.  See wl_home() for 
   more info.

 wl_root - See wl_home().

 source_dir - The path to the application source.  See
   source_dir().

 source_root - See source_dir().

 prop_name_base - The prefix to use for calculated system
   properties.

Valid options for WDIG::Weblogic::Server, WDIG::Weblogic::Cmd
and WDIG::Run will be passed on appropriately.

=item start(%opt)

Starts each server, will fail on first error; i.e. if the
first server does not start, the second server will not
be started either.

=item stop(%opt)

As start() but stops each server.

=item restart(%opt)

Will restart each server.

=item ping(%opt)

Pings each server.

=item test(%opt)

Tests each server by requesting a test page.

=item kill(%opt)

Kill each server.

=item refresh(%opt)

Will refresh the application content and configuration
for each server from source_dir().

=item flushcache(%opt)

Will flush the content cache for each server.

=item wl_home()

This method selects the correct Weblogic home to use.  The wl_home
must contain a C<config/$DOMAIN_NAME/config.xml> file.  If the
C<wl_home> option is specified, that value is used.  If not, then
C<wl_root/$DOMAIN_NAME/$APP_NAME> is tried.  Next C<$ENV{WL_HOME}>,
and finally C<source_dir()>

=item source_dir()

Determins the source for application content and configuration.
This may often be the same as wl_home() but may be a seperate
directory from which the working set of files is copied.  If
set, the C<source_dir> option is used, followed by 
C<source_root/$DOMAIN_NAME/$APP_NAME> and then wl_home().

=item path()

Returns the path to the application content as used by Weblogic.
This is the root of the enterprise application - EAR.

=item system_properties()

Determines the system properties for the application by searching
for files called C<application.properties> and C<$APP_NAME.properties>
and loading the values to a hash.

=item config()

Returns the application configuration data structure as defined 
by WDIG::Weblogic::Config.

=item targets()

Returns the list of target servers.

=item webapp()

Returns a list of web app components within this app.

=item ejb()

Returns a list of EJB components within this app.

=item jdbc_config($pool_name)

Queries the first server in the target list for it's JDBC 
pool configuration.

=back

=head1 ERROR HANDLING

Each method will throw an Error object when an exception is caught.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Application.pm,v $
 Revision 1.6  2003/09/03 20:21:05  pegan
 Removed unnecessary WDIG::Content module references

 Revision 1.5  2003/09/03 13:40:39  pegan
 Fixed ordering bug with prop_name_base

 Revision 1.4  2003/08/26 19:17:54  pegan
 Added test() & jdbc_config() methods

 Revision 1.3  2003/07/14 18:59:11  pegan
 Added Log4perl support
 Content refresh() is now delegated to the server
 system_properties() now uses Config::Properties rather than
   plain old strings
 source_dir() added as a seperate method to path()
 wl_home() now tries source_dir() too.

 Revision 1.2  2003/06/30 15:15:19  pegan
 Added targets(), webapp(), ejb(), refresh() & flushcache() methods.
 Moved wl_home calculation to seperate wl_home() method.
 Renamed get_properties() to system_properties().

 Revision 1.1  2003/05/09 21:22:46  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Weblogic::Application;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw();
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Carp qw(carp cluck);
use IO::File;
use Error qw(:try);
use File::Basename;
use File::Spec::Functions qw(catfile catdir updir file_name_is_absolute);
use XML::Simple qw(:strict);
use Config::Properties;
use Log::Log4perl qw(:nowarn);
use WDIG::File::Slurp;
use WDIG::Weblogic::Config;
use WDIG::Weblogic::Server;

my $Properties_Files = 'config/application.properties config/%s.properties';

my $Prop_Name_Base = 'WDIG.Weblogic';
my $Prop_Name_Path = 'Path';
my $Refresh_Timestamp_File = 'REFRESH';


######################################################################
# new($name,$domain,%opt) - application constructor.  Populate unset
# options with some appropriate defaults and retrieve the application
# configuration from WDIG::Weblogic::Config.
#

sub new
{
 my $self = shift();
 my $name = shift();
 my $domain = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $app = {};
 
 $app->{logger} = Log::Log4perl->get_logger();
 $app->{name} = $name;
 $app->{domain} = $domain;
 %{$app->{opt}} = %opt;

 bless($app, $package);

 $app->{logger}->debug("WDIG::Weblogic::Application->new($name,$domain)");

 $app->apply_defaults($opt{defaults}, $app->{opt}) if ( $opt{defaults} );

 if ( !$app->wl_home() ) {
   throw Error::Simple("Unable to determine wl_home for $domain:$name");
 } else {
   $app->{logger}->debug("$name: Using wl_home '".$app->wl_home()."'");
 }

 return ($app);
}


######################################################################
# tell_servers(%opt) - calls the appropiate method on each server
# in the Target list. Re-throws server errors.  The method is 
# selected with a switch (implemented with a for()) instead of
# an eval() because this would interrupt the try/catch block.
#

sub tell_servers
{
 my $app = shift();
 my $cmd = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$app->{opt}}, %opt );

 throw Error::Simple("No target servers for $app->{name}") if ( !$app->targets() );

 foreach my $server_name ( $app->targets() ) {

   try {

     my $server = new WDIG::Weblogic::Server($server_name, $app->{domain}, %opt);

     for ($cmd) {
       /^start/      and $success = $server->start();
       /^stop/       and $success = $server->stop();
       /^restart/    and $success = $server->restart();
       /^ping/       and $success = $server->ping();
       /^test/       and $success = $server->test();
       /^kill/       and $success = $server->kill();
       /^refresh/    and $success = $server->refresh();
       /^flushcache/ and $success = $server->flushcache();
     }

   } catch Error with {

     my $error = shift();
     throw $error;

   };

 }
 
 return ($success);
}


######################################################################
# system_properties($source_dir,$files) - searches for properties
# files and returns the properties as a hash.
#

sub system_properties
{
 my $app = shift();
 my @search_path = ( shift()||0 );
 my $files = shift() || sprintf($Properties_Files,$app->{name});
 my %opt = @_;

 %opt = ( %{$app->{opt}}, %opt );

 my $props = new Config::Properties();

 push(@search_path, $app->wl_home(), $app->source_dir()||0, dirname($app->source_dir()||0) );
 { my %uniq; @uniq{@search_path}=(); @search_path=grep($_,keys(%uniq)); }

 foreach my $file ( split(/[\s,]/, $files) ) {
   if ( file_name_is_absolute($file) and -r $file ) {
     $app->{logger}->debug("Reading properties file '$file'");
     $props->load(new IO::File($file));
   } else {
     foreach my $dir ( @search_path ) {
       my $abs_file = catfile($dir, $file);
       if ( -r $abs_file ) {
         $app->{logger}->debug("Reading properties file '$abs_file'");
         $props->load(new IO::File($abs_file));
       }
     }
   }
 }

 my $prop_name_base = $opt{prop_name_base} || $Prop_Name_Base;
 $props->setProperty("$prop_name_base.$Prop_Name_Path", $app->path(home=>$app->source_dir()));

 return ($props->getProperties());
}


######################################################################
# apply_defaults($defaults,\%opt) - Combines default options with
# the current values.
#

sub apply_defaults
{
 my $app = shift();
 my $defaults = shift();
 my $opt = shift();

 if ( ref($defaults) ne 'HASH' ) {

   carp("Unable to parse defaults '$defaults': $@\n")
     if ( ! eval { $defaults = XMLin($defaults, ForceArray=>0, KeyAttr=>{}) } );

 }

 if ( $defaults and ref($defaults) eq 'HASH' ) {
   %$opt = ( %$defaults, %$opt );
   return (1);
 } else {
   return (undef);
 }
}


######################################################################
# wl_home() - Determines the Weblogic home, ie the directory that will
# contain the config sub-directory.
#

sub wl_home
{
 my $app = shift();
 my $name = shift() || $app->{name};
 my $domain = shift() || $app->{domain};
 my %opt = @_;
 my $wl_home = $app->{wl_home};

 return ($wl_home) if ( $wl_home );
 
 %opt = ( %{$app->{opt}}, %opt );

 if ( $opt{wl_home} ) {
 
   $wl_home = $opt{wl_home};

 } elsif ( $opt{wl_root} ) {

   if ( -d (my $dir = catdir($opt{wl_root}, $domain, $name)) ) {
     $wl_home = $dir;
   } elsif ( -d ($dir = catdir($opt{wl_root}, $domain)) ) {
     $wl_home = $dir;
   } else {
     $wl_home = $opt{wl_root};
   }

 } else {

   $wl_home = $ENV{WL_HOME} || '.';

 }

 my $config_file_path = catfile('config', $domain, 'config.xml');
 
 if ( !-f catfile($wl_home, $config_file_path) ) {

   $app->{logger}->debug("No config file found in '$wl_home'");
   $wl_home = undef;

   # Can't use source_dir() here because it's too early :-(
   if ( $opt{source_dir} ) {
     $wl_home = $opt{source_dir};
   } elsif ( $opt{source_root} ) {
     if ( -d (my $dir = catdir($opt{source_root}, $domain, $name)) ) {
       $wl_home = $dir;
     } elsif ( -d ($dir = catdir($opt{source_root}, $domain)) ) {
       $wl_home = $dir;
     } else {
       $wl_home = $opt{source_root};
     }
   }

   if ( $wl_home ) {
     $app->{logger}->debug("Trying source dir '$wl_home'");
     if ( !-f catfile($wl_home, $config_file_path) ) {
       $wl_home = undef;
     }
   }

 }

 $app->{wl_home} = $wl_home;

 return ($wl_home);
}


######################################################################
# config() - Returns the app configuration data structure.
#

sub config
{
 my $app = shift();

 if ( !$app->{config} ) {

   my $domain_config = new WDIG::Weblogic::Config($app->{domain}, wl_home=>$app->wl_home());
   $app->{config} = $domain_config->application($app->{name});

 }

 return ($app->{config}); 
}


######################################################################
# targets() - Returns the list of target servers
#

sub targets
{
 my $app = shift();
 my $targets = $app->config()->{Target};
 return (wantarray ? @{$targets} : $targets); 
}


######################################################################
# path() - Returns the path to the application content.
#

sub path
{
 my $app = shift();
 my %opt = @_;

 return ($app->{path}) if ( $app->{path} and !$opt{home} );

 my $app_path = $app->config()->{Path};
 
 if ( !file_name_is_absolute($app_path) ) {

   $app_path = catdir($opt{home} || $app->wl_home(), $app_path);

 }
 
 if ( !$opt{home} ) {
   $app->{logger}->debug("Source app path set to '$app_path'");
   $app->{path} = $app_path;
 }
 
 return ($app_path);
}


######################################################################
# source_dir() - Returns the path to the application source.
#

sub source_dir
{
 my $app = shift();
 my %opt = @_;
 my $source_dir = $app->{source_dir};

 return ($source_dir) if ( $source_dir );

 %opt = ( %{$app->{opt}}, %opt );
 
 if ( $opt{source_dir} ) {
 
   $source_dir = $opt{source_dir};

 } elsif ( $opt{source_root} ) {

   if ( -d (my $dir = catdir($opt{source_root}, $app->{domain}, $app->{name})) ) {
     $source_dir = $dir;
   } elsif ( -d ($dir = catdir($opt{source_root}, $app->{domain})) ) {
     $source_dir = $dir;
   } else {
     $source_dir = $opt{source_root};
   }

 } else {

   $source_dir = $app->wl_home();

 }
 
 $app->{logger}->debug("Source dir set to '$source_dir'");
 
 $app->{source_dir} = $source_dir;
 
 return ($source_dir);
}


######################################################################
# webapp() - Returns a list of web app components within this app
#

sub webapp
{
 my $app = shift();
 my %opt = @_;

 my @webapps = ();

 while ( my ($name, $component) = each(%{$app->config()->{Component}}) ) {
   if ( $component->{Type} eq 'WebAppComponent' ) {
     my $webapp = { Name=>$name, %$component };
     push(@webapps, $webapp);
   }
 }

 return (wantarray ? @webapps : \@webapps);
}


######################################################################
# ejb() - Returns a list of EJB components within this app
#

sub ejb
{
 my $app = shift();
 my %opt = @_;

 my @ejbs = ();

 while ( my ($name, $component) = each(%{$app->config()->{Component}}) ) {
   if ( $component->{Type} eq 'EJBComponent' ) {
     my $ejb = { Name=>$name, %$component };
     push(@ejbs, $ejb);
   }
 }

 return (wantarray ? @ejbs : \@ejbs);
}


######################################################################
# jdbc_config($pool_name) - Returns the jdbc connection pool config.
# Unlike the other methods that are passed on to the servers, this
# method uses only the first server in the target list.
#

sub jdbc_config
{
 my $app = shift();
 my $pool_name = shift();
 my %opt = @_;

 %opt = ( %{$app->{opt}}, %opt );

 throw Error::Simple("No target servers for $app->{name}") if ( !$app->targets() );

 my $server = new WDIG::Weblogic::Server(($app->targets())[0], $app->{domain}, %opt);

 return ($server->jdbc_config($pool_name));
}


######################################################################
# Stubs for commands -

sub start      { my $app = shift(); return ($app->tell_servers('start',@_)); }
sub stop       { my $app = shift(); return ($app->tell_servers('stop',@_)); }
sub restart    { my $app = shift(); return ($app->tell_servers('restart',@_)); }
sub ping       { my $app = shift(); return ($app->tell_servers('ping',@_)); }
sub test       { my $app = shift(); return ($app->tell_servers('test',@_)); }
sub kill       { my $app = shift(); return ($app->tell_servers('kill',@_)); }
sub flushcache { my $app = shift(); return ($app->tell_servers('flushcache',app=>$app->{name},@_)); }
sub refresh    { my $app = shift(); return ($app->tell_servers('refresh',app=>$app->{name},@_)); }


1;
__END__


