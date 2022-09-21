
# $Id$

=head1 NAME

WDIG::Weblogic::Server - OO interface for the management of a Weblogic server

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.egan@disney.com
 Date: 2003/05/02

=head1 SYNOPSIS

 use WDIG::Weblogic::Server;

 my $server = new WDIG::Weblogic::Server($serverName, $domainName);

 my $host = $server->machine();

 $server->start();

 $server->stop() if ( $server->ping() );

=head1 DESCRIPTION

The Server class implements an OO interface for the management of a 
Weblogic server.  Configuration information is read using WDIG::Weblogic::Config 
and WDIG::Weblogic::Cmd is used to perform any commands.  If the
server is targeted at a remote machine, ssh is used to call the
corresponding method remotely.  The following methods are available.

=over 4

=item new($name,$domain,%opt)

The constructor creates a new server object based on the configuration
found in the specified domain.  The following options are supported by
both the constructor and individual methods.  Options to the methods
will override those given to the constructor.

 defaults - A hash reference or an object suitable for
   XML::Simple (XML doc or file) that will be used to
   specify default options.  The XML::Simple options
   used are "ForceArray=>0, KeyAttr=>{}".

 wl_home - Specifies the directory where the Weblogic
   config sub-dir can be found.  See wl_home() for 
   more info.

 wl_root - See wl_home().

 prop_name_base - The prefix to use for derived system
   properties.

 passwd - Specifies the Weblogic system password.  See
   passwd() for more info.

 ssh - A hash reference to SSH config options.  Must 
   contain path, options and cmd elements.  Default:
   { path=>"ssh", options=>"-T -a -x", cmd=>"perl -w" }
   
 java_options - java runtime options.  See java_options().

 properties - See system_properties().

Valid options for WDIG::Weblogic::Cmd and WDIG::Run 
will be passed on appropriately.

=item start(%opt)

Starts a Weblogic server using WDIG::Weblogic::Cmd::start_server().
If the server is assigned to a machine in the Weblogic configuration
and that machine is not the local host, then ssh is used to start
the server remotely.

=item stop(%opt)

Stops a Weblogic server using WDIG::Weblogic::Cmd::stop_server(),
specifying the correct weblogic log to check for shutdown success.

=item restart(%opt)

Calls stop() followed by start().

=item ping()

Returns true if the server responds to a ping.  This command is
always performed locally.

=item test()

Tests the server by requesting a test page and verifying the
response.  The C<test_page> and C<test_success_string> options
can be used to override the dafaults, 'monitor.jsp' and
'STATUS: OK' respectively.  Returns true if the response
matches the success pattern.

=item kill()

Sends a kill signal to the server using the watchdog process spawned
by WDIG::Run.

=item flushcache()

Deletes each of the servers cache (compilation & staging) directories.

=item refresh()

Copies updated application content from the application C<source_dir()>
and restarts the server if neccessary.

=item wl_home()

This method selects the correct Weblogic home to use.  The wl_home
must contain a C<config/$DOMAIN_NAME/config.xml> file.  If the
C<wl_home> option is specified, that value is used.  If not, then
C<wl_root/$DOMAIN_NAME/$APP_NAME> is tried.  Next C<$ENV{WL_HOME}>,
and finally application(s) wl_home() is checked.

=item system_properties()

Determines the system properties for the server by searching
for files called C<system.properties> and C<$SERVER_NAME.properties>
and loading the values to a hash.  Application properties are
included and certain additional properties are derived.  The
C<properties> option can be used to add to the list.

=item java_options()

Determines the java options for the server by searching
for files called C<java.options> and C<$SERVER_NAME.options>
and reading the first file found to a string.

=item passwd()

If the C<passwd> option is used then it is taken to be the
password.  If it begins with a '/' then it is treated as a
file from which the password is read.  The config directory
is also searched for a passwd file.  If not is found then
the user is asked to enter the system password.

=item log_dir()

Used to decide where log files are stored.

=item config()

Returns the server configuration data structure as defined by 
WDIG::Weblogic::Config.

=item machine()

Returns the machine name for this server.

=item port()

Returns the listen port for this server.

=item apps()

Returns list of apps targetted at this server.

=item jdbc_config($pool_name)

Returns the JDBC connection pool config by querying the
running server for JDBCConnectionPoolConfig type mbeans.
The mbean properties are augmented with elements more usefull 
for using as WDIG::DB::SqlInterpreter connection properties.
If no pool_name is specified, then a hash containing all
configured pool is returned.

=back

=head1 ERROR HANDLING

Each method will throw an Error object when an exception is caught.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Server.pm,v $
 Revision 1.8  2003/09/03 13:41:26  pegan
 Fixed ordering bug with prop_name_base

 Revision 1.7  2003/08/27 02:42:44  pegan
 Added options to Config::Runtime call

 Revision 1.6  2003/08/26 20:34:02  pegan
 removed ssh_log_file references

 Revision 1.5  2003/08/26 20:04:50  pegan
 ssh_cmd() now uses cmd exit code to catch exception
   and no longer logs to a seperate file
 the ssh "rpc stub" now uses the log_conf_file option
 added test() method and call to it at the end of a refresh()
 added jdbc_config() method and underlying mbean methods

 Revision 1.4  2003/07/14 19:38:25  pegan
 Added refresh() functionality
 passwd() & log_dir() migrated from Config
 Log4perl used in place of print/$opt{feedback}
 Stuff like log locations deffered until after instansiation
 system_properties() now uses Config::Properties
 wl_home() now falls back to application paths.

 Revision 1.3  2003/07/03 20:39:51  pegan
 Only one java options file now loaded
 Now checks that passwd file can be read

 Revision 1.2  2003/07/01 04:22:15  pegan
 Added machine() & port() methods
 Renamed system_properties() & java_options() methods
 Moved WL_HOME calculation to wl_home() method
 Added flushcache() method

 Revision 1.1  2003/05/09 21:15:32  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Weblogic::Server;

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
use IO::File;
use Error qw(:try);
use Net::Domain qw(hostname hostfqdn);
use Data::Dumper;
use File::Basename;
use File::Spec::Functions qw(catfile catdir file_name_is_absolute);
use File::Rsync;
use XML::Simple qw(:strict);
use Config::Properties;
use Log::Log4perl qw(:nowarn);
use LWP::UserAgent;
use WDIG::File::Slurp;
use WDIG::File::Cmd;
use WDIG::Run;
use WDIG::Weblogic::Config;
use WDIG::Weblogic::Config::Runtime;
use WDIG::Weblogic::Application;
use WDIG::Weblogic::Cmd;


my $SSH_Config = { path=>"ssh", options=>"-T -a -x", cmd=>"perl -w" };
my $SSH_Script_Template = join('',<DATA>);

my $SSH_Success_String = "SUCCESS";
my $SSH_Error_String = "^Server error";
my $SSH_Timeout = 240;

my $Java_Options_Files = "config/%s.options config/java.options";
my $Properties_Files = "config/%s.properties config/system.properties";

my $Prop_Name_Base = 'WDIG.Weblogic';
my $Prop_Name_Port = 'ListenPort';
my $Prop_Name_Machine = 'Machine';
my $Prop_Name_FrontendHost = 'FrontendHost';
my $Prop_Name_FrontendHTTPPort = 'FrontendHTTPPort';

my $Default_Test_Page = 'monitor.jsp';
my $Default_Test_Success_String = 'STATUS: OK';

my $Log_Root = 'logs';

my $Passwd_File = 'passwd';
my $Passwd_Search_Path = 'config . ..';

my $Restart_Files = '(^config\/.*\.properties$|^config\/.*\.xml$|-INF\/.*\.xml$|\.jar$)';
my @Refresh_Excludes = ( '*.cache' );


######################################################################
# new($name,$domain,%opt) - server constructor.  Retrieves the server
# configuration using WDIG::Weblogic::Config and sets unset options
# to some appropriate defaults.
#

sub new
{
 my $self = shift();
 my $name = shift();
 my $domain = shift();
 my %opt = @_;
 my $package = ref($self) || $self;
 my $server = {};
 
 $server->{logger} = Log::Log4perl->get_logger();
 $server->{name} = $name;
 $server->{domain} = $domain;
 %{$server->{opt}} = %opt;

 bless($server, $package);

 $server->{logger}->debug("WDIG::Weblogic::Server->new($name,$domain)");

 $server->apply_defaults($opt{defaults}, $server->{opt}) if ( $opt{defaults} );

 if ( !$server->wl_home() ) {
   throw Error::Simple("Unable to determine wl_home for $domain:$name");
 } else {
   $server->{logger}->info("$name: Using wl_home '".$server->wl_home()."'");
 }

 if ( $opt{copy_config} ) {
   cp($opt{copy_config}, catdir($server->wl_home(),'config',$domain))
     or $server->{logger}->warn("Could not copy $opt{copy_config}");
 }

 $server->{opt}->{show_output} = $server->{logger}->is_info() &&
                                 Log::Log4perl->get_logger("")->has_appenders();

 return ($server);
}


######################################################################
# start(%opt) - start the server using WDIG::Weblogic::Cmd or ssh
#

sub start
{
 my $server = shift();
 my %opt = @_;
 my $name = $server->{name};
 my $domain = $server->{domain};
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt );

 return($server->ssh_cmd('start', %opt)) if ( $server->is_remote() );

 $opt{wl_home} = $server->wl_home();
 $opt{java_options} = $server->java_options();
 $opt{properties} = $server->system_properties();
 $opt{log_file} = catfile($server->log_dir('stdout'),$name.".log") if ( !$opt{log_file} );
 $opt{pid_file} = catfile($server->log_dir('pid'),$name) if ( !$opt{pid_file} );

 $server->{logger}->info("Starting $name on ".hostname());

 $success = start_server($name, $domain, $server->passwd(), %opt) or
        throw Error::Simple("Server error: ".$WDIG::Weblogic::Cmd::errstr);

 return ($success);
}


######################################################################
# stop(%opt) - stop the server using WDIG::Weblogic::Cmd
#

sub stop
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );

 return ($server->ssh_cmd('stop', %opt)) if ( $server->is_remote() );

 $opt{check_log_file} = $server->config()->{LogFileName} if ( !defined($opt{check_log_file}) );
 $opt{wl_home} = $server->wl_home();

 $server->{logger}->info("Stopping $server->{name} on ".hostname());

 $success = stop_server("localhost", $server->port(), $server->passwd(), %opt) or
        throw Error::Simple("Server error: ".$WDIG::Weblogic::Cmd::errstr);

 $server->{logger}->info("Shutdown successful");

 return ($success);
}


######################################################################
# restart(%opt) - restart the server by calling stop() then start()
#

sub restart
{
 my $server = shift();
 my %opt = @_;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );

 return ($server->ssh_cmd('restart', %opt)) if ( $server->is_remote() );

 my $success = $server->stop() and $server->start();

 return ($success);
}


######################################################################
# ping(%opt) - ping the server using WDIG::Weblogic::Cmd
#

sub ping
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );

 my $host = $server->machine() || "localhost";

 $server->{logger}->info("Pinging $server->{name} on $host");

 $opt{show_output} = $opt{debug} && !$opt{throw_error};

 $success = ping_server($host, $server->port(), $server->passwd(), %opt);
    
 throw Error::Simple("$server->{name} is alive")
            if ( $success and $opt{throw_error} and $opt{throw_error} eq 'alive' );
 throw Error::Simple("$server->{name} is dead")
            if ( !$success and $opt{throw_error} and $opt{throw_error} ne 'alive' );

 if ( $success ) {
   $server->{logger}->info("$server->{name} is alive");
 } else {
   $server->{logger}->error("$server->{name} did not respond to ping");
 }

 return ($success);
}


######################################################################
# test(%opt) - test the server by requesting monitor.jsp
#

sub test
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );

 my $browser = new LWP::UserAgent(timeout=>30);

 my $host = $server->machine() || "localhost";
 my $test_page = $opt{test_page} || $Default_Test_Page;
 my $success_string = $opt{test_success_string} || $Default_Test_Success_String;
 my $url = 'http://'.$host.':'.$server->port().'/'.$test_page;

 $server->{logger}->info("Requesting $url");

 my $request = new HTTP::Request(GET=>$url);
 my $result = $browser->request($request);
 my $response = $result->content();

 $response =~ s/^\s+//g; $response =~ s/\s+$//g;

 $success = $result->is_success() && $response =~ /$success_string/;

 throw Error::Simple("Got response: '$response'")
            if ( $success and $opt{throw_error} and $opt{throw_error} eq 'alive' );
 throw Error::Simple("$url returned:\n$response")
            if ( !$success and $opt{throw_error} and $opt{throw_error} ne 'alive' );

 if ( $result->is_success() ) {
   $server->{logger}->info("Got response: '$response'");
 } else {
   $server->{logger}->error("Got response: '".$result->message()."'");
 }

 return ($success);
}


######################################################################
# kill(%opt) - sends a TERM signal to the server watchdog
#

sub kill
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );
 
 return ($server->ssh_cmd('kill', %opt)) if ( $server->is_remote() );

 $server->{logger}->info("Sending KILL to $server->{name} on ".hostname());

 my $pid_file = $opt{pid_file} || catfile($server->log_dir('pid'),$server->{name});
 if ( -r $pid_file ) {
   my $pid = readFile($pid_file);
   $success = CORE::kill("TERM",$pid);
 } else {
   $server->{logger}->warn("$pid_file not found");
 }

 return ($success);
}


######################################################################
# flushcache(%opt) - deletes the server's cache directories
# There's a lot of hard-coded stuff here that probably belongs
# somewhere like the config class.
#

sub flushcache
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );

 return ($server->ssh_cmd('flushcache', %opt)) if ( $server->is_remote() );

 $server->{logger}->info("Deleting cache dirs for $server->{name} on ".hostname());

 my @files = ();

 foreach my $app_name ( $opt{app} || $server->apps() ) {
   try {
     my $app = new WDIG::Weblogic::Application($app_name, $server->{domain}, %opt, wl_home=>$server->wl_home());
     foreach my $webapp ( $app->webapp() ) {
       my $tmp_dir = catdir($app->path(), $webapp->{URI}, 'WEB-INF', '_tmp_war');
       push(@files, join('_',$tmp_dir,$webapp->{Name}),
                    join('_',$tmp_dir,$server->{domain},$server->{name},$webapp->{Name}));
     }
     foreach my $ejb ( $app->ejb() ) {
       push(@files, catdir($server->wl_home(), ($ejb->{TmpPath}||'tmp_ejb').hostfqdn().$server->port() ) );
     }
   } catch Error with {
     my $error = shift();
     $server->{logger}->error($error->text());
   };
 }

 push(@files, catdir($server->wl_home(), 'config', $server->{domain}, 'applications', '.wlnotdelete') );
 push(@files, catdir($server->wl_home(), 'config', $server->{domain}, 'applications', '.wlnotdelete_'.$server->{name}) );
 push(@files, catdir($server->wl_home(), 'config', $server->{domain}, '.wlstaging') );

 $success = 1;
 foreach my $file ( @files ) {
   if ( -e $file ) {
     $server->{logger}->info("Deleting $file");
     rm($file, recurse=>1) or $success = undef;
   }
 }

 return ($success);
}


######################################################################
# refresh() - a refresh is an application content thing
#

sub refresh
{
 my $server = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0 );
 
 return ($server->ssh_cmd('refresh', %opt)) if ( $server->is_remote() );

 $server->{logger}->info("Refreshing content for $server->{name} on ".hostname());
 my @refreshed_files = ();

 foreach my $app_name ( $opt{app} || $server->apps() ) {

   my ( $src, $app_src, $app_dest ) = undef;

   try {
     my $app = new WDIG::Weblogic::Application($app_name, $server->{domain}, %opt, wl_home=>$server->wl_home());
     $src = $app->source_dir();
     $app_src = $app->path(home=>$src);
     $app_dest = $app->path();
   } catch Error with {
     my $error = shift();
     $server->{logger}->error($error->text());
   };
   
   throw Error::Simple("App src & dest are the same: '$app_src'") if ( $app_src eq $app_dest );

   # my $rsh = $opt{ssh}->{path}." ".$opt{ssh}->{options};
   my $rsync = new File::Rsync(archive=>1, 'log-format'=>'%f', exclude=>\@Refresh_Excludes, debug=>0);

   chdir($app_src) or throw Error::Simple("$app_src: $!");
   $server->{logger}->info("rsync -a ".$app_src."/ ".$app_dest);
   $success = $rsync->exec(src=>'./', dest=>$app_dest) or
       throw Error::Simple("Rsync error: ".join(" ",$rsync->err));
   
   $server->{logger}->debug("App files: \n".join("", $rsync->out));
   push(@refreshed_files, $rsync->out); chomp(@refreshed_files);

   chdir($src) or throw Error::Simple("$src: $!");
   $server->{logger}->info("rsync -a ".catdir($src,'config')." ".$server->wl_home());
   $success = $rsync->exec(src=>'config', dest=>$server->wl_home()) or
       throw Error::Simple("Rsync error: ".join(" ",$rsync->err));
   
   $server->{logger}->debug("Config files: \n".join("", $rsync->out));
   push(@refreshed_files, $rsync->out); chomp(@refreshed_files);

 }

 my $server_is_alive = $server->ping();

 if ( $server_is_alive and grep(/$Restart_Files/, @refreshed_files) ) {
   $server->{logger}->warn("$server->{name} needs to be restarted");
   $success = $server->restart() ? "RESTART" : undef;
 }

 if ( $server_is_alive and !$opt{no_test} ) {
   try {
     $server->test(throw_error=>1);
   } catch Error with {
     my $error = shift();
     $server->kill();
     throw $error;
   };
 }

 return ($success);
}


######################################################################
# ssh_cmd() - starts a remote perl using SSH and sends the code to
# execute the method with the specified options.
#

sub ssh_cmd
{
 my $server = shift();
 my $method = shift();
 my %opt = @_;
 my $success = undef;

 throw Error::Simple("Recursion detected on ".hostname()) if ( $opt{_is_ssh_call} );
 $opt{_is_ssh_call} = 1;

 $opt{log_level} = $server->{logger}->level();

 $server->{logger}->debug("Calling $method on ".$server->machine());

 my %ssh_config = %{$SSH_Config};
 if ( $server->{opt}->{ssh} and ref($server->{opt}->{ssh}) eq 'HASH' ) {
   %ssh_config = %{$server->{opt}->{ssh}};
 }

 my @ssh_cmd = ( $ssh_config{path},
 	     	 split(/\s+/, $ssh_config{options} ),
 	     	 $server->machine(),
             	 $ssh_config{cmd} );

 local $Data::Dumper::Terse = 1; 
 my $input = sprintf($SSH_Script_Template, $server->{name},
                                           $server->{domain},
					   Dumper(\%opt),
					   $method,
					   $SSH_Success_String);

 $server->{logger}->debug("SSH Cmd: ", join(" ",@ssh_cmd));

 my $ssh_output = "";
 $success = run_cmd(\@ssh_cmd, timeout => $SSH_Timeout,
                               input => $input, 
                               output => \$ssh_output,
			       show_output => ( $server->{opt}->{show_output} && -t ),
			       wd => $server->wl_home()) or
 	throw Error::Simple($WDIG::Run::errstr);

 return ($success ? $ssh_output : undef);
}


######################################################################
# is_remote() - determine if this server is run on a remote machine
#

sub is_remote
{
 my $server = shift();
 my $host = $server->machine();
 return ( 0 ) if ( $server->{opt}->{_is_ssh_call} ); # Force local if after SSH call
 return ( $host and $host ne "localhost" and $host ne hostname() );
}


######################################################################
# java_options($source_dir,$files) - searches the specified dir
# for options files and loads the first one found to a string
#

sub java_options
{
 my $server = shift();
 my @search_path = ( shift()||0 );
 my $files = shift() || sprintf($Java_Options_Files,$server->{name});
 my %opt = @_;
 my $options = $server->{java_options};

 return $options if ( $options );

 %opt = ( %{$server->{opt}}, %opt );

 if ( $opt{java_options} and $opt{java_options} =~ /^\// and -f $opt{java_options} ) {

   $server->{logger}->debug("Reading options file '$opt{java_options}'");
   $options .= readFile($opt{java_options});
  
 } elsif ( $opt{java_options} ) {

   $options = $opt{java_options};

 } else {

   push(@search_path, $server->wl_home() );
   { my %uniq; @uniq{@search_path}=(); @search_path=grep($_,keys(%uniq)); }

   foreach my $file ( split(/[\s,]/, $files) ) {
     if ( file_name_is_absolute($file) and -r $file ) {
       $server->{logger}->debug("Reading options file '$file'");
       $options .= readFile($file);
     } else {
       foreach my $dir ( @search_path ) {
         my $abs_file = catfile($dir, $file);
         if ( -r $abs_file ) {
           $server->{logger}->debug("Reading options file '$abs_file'");
           $options .= readFile($abs_file);
           last;
         }
       }
     }
     last if ( $options );
   }

 }

 if ( $options ) {
   $options =~ s/^\s*#.*$//mg;
   $options =~ s/\s+/ /g;
 }

 $server->{java_options} = $options;

 return ($options);
}


######################################################################
# system_properties($source_dir,$files) - searches the specified dir
# for properties files and loads them to a hash.
#

sub system_properties
{
 my $server = shift();
 my @search_path = ( shift()||0 );
 my $files = shift() || sprintf($Properties_Files,$server->{name});
 my %opt = @_;

 return ($server->{system_properties}) if ( $server->{system_properties} );

 %opt = ( %{$server->{opt}}, %opt );

 my $props = new Config::Properties();

 if ( $opt{properties} ) {

   if ( $opt{properties} =~ /^\// and -f $opt{properties} ) {
     $server->{logger}->debug("Reading properties file '$opt{properties}'");
     # $props->load(new IO::File($opt{properties}));
     my $p=new IO::File($opt{properties}); $props->load($p); $p->close();
     $props = $props->getProperties();
   } else {
     $props = $opt{properties};
   }

   $server->{system_properties} = $props;
   return ($props);

 }

 push(@search_path, $server->wl_home() );
 { my %uniq; @uniq{@search_path}=(); @search_path=grep($_,keys(%uniq)); }

 foreach my $file ( split(/[\s,]/, $files) ) {
   if ( file_name_is_absolute($file) and -r $file ) {
     $server->{logger}->debug("Reading properties file '$file'");
     # $props->load(new IO::File($file));
     my $p=new IO::File($file); $props->load($p); $p->close();
   } else {
     foreach my $dir ( @search_path ) {
       my $abs_file = catfile($dir, $file);
       if ( -r $abs_file ) {
         $server->{logger}->debug("Reading properties file '$abs_file'");
         # $props->load(new IO::File($abs_file));
         my $p=new IO::File($abs_file); $props->load($p); $p->close();
       }
     }
   }
 }

 my $prop_name_base = $opt{prop_name_base} || $Prop_Name_Base;
 $props->setProperty("$prop_name_base.$Prop_Name_Port", $server->port());
 $props->setProperty("$prop_name_base.$Prop_Name_Machine", $server->machine());
 $props->setProperty("$prop_name_base.$Prop_Name_FrontendHost", $server->config()->{WebServer}->{FrontendHost} || "unknown");
 $props->setProperty("$prop_name_base.$Prop_Name_FrontendHTTPPort", $server->config()->{WebServer}->{FrontendHTTPPort} || 80);

 $props = $props->getProperties();
 
 foreach my $app_name ( $server->apps() ) {
   try {
     my $app = new WDIG::Weblogic::Application($app_name, $server->{domain}, %opt, wl_home=>$server->wl_home());
     my $app_props = $app->system_properties();
     $props = { %$props, %$app_props } if ( $app_props );
   } catch Error with {
     my $error = shift();
     $server->{logger}->error($error->text());
   };
 }

 $server->{system_properties} = $props;

 return ($props);
}


######################################################################
# apply_defaults($defaults,\%opt) - Combines default options with
# the current values.
#

sub apply_defaults
{
 my $server = shift();
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
 my $server = shift();
 my $name = shift() || $server->{name};
 my $domain = shift() || $server->{domain};
 my %opt = @_;
 my $wl_home = $server->{wl_home};

 return ($wl_home) if ( $wl_home );
 
 %opt = ( %{$server->{opt}}, %opt );

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

   $wl_home = $ENV{WL_HOME} || '.' ;

 }

 my $config_file_path = catfile('config', $domain, 'config.xml');
 
 if ( !-f catfile($wl_home, $config_file_path) ) {

   $server->{logger}->debug("No config file found in '$wl_home'");
   $wl_home = undef;

   # Too early for $server->apps()
   if ( $name =~ /^(.*?)\d+$/ ) {
     my $app_name = $1;
     $server->{logger}->debug("Trying app name '$app_name'.");
     try {
       my $app = new WDIG::Weblogic::Application($app_name, $domain, %opt);
       $wl_home = $app->wl_home();
     } catch Error with {
       my $error = shift();
       $server->{logger}->error("Application->new() failed: ".$error->text());
     };
   }

 }

 $server->{wl_home} = $wl_home;
 
 return ($wl_home);
}


######################################################################
# log_dir($log_type) - Returns the root dir for the specified log
# file type.
#

sub log_dir
{
 my $server = shift();
 my $type = shift() || '';
 my %opt = @_;
 my $log_dir;
 
 %opt = ( %{$server->{opt}}, %opt );

 my $log_root = file_name_is_absolute($Log_Root) ? $Log_Root : catdir($server->wl_home(), $Log_Root);

 if ( -d catdir($log_root, $server->{domain}) ) {
   $log_dir = catdir($log_root, $server->{domain}, $type);
 } else {
   $log_dir = catdir($log_root, $type);
 }

 # Should we throw an exception?
 $server->{logger}->error("Log directory '$log_dir' does not exist") if ( !-d $log_dir );

 return ($log_dir);
}


######################################################################
# passwd() - Returns the Weblogic system password.
#

sub passwd
{ 
 my $server = shift();
 my %opt = @_;
 my $passwd = $server->{passwd};

 return ($passwd) if ( $passwd );

 %opt = ( %{$server->{opt}}, %opt );

 if ( $opt{passwd} and $opt{passwd} =~ /^\// ) { # and -f $opt{passwd}

   my $file = $opt{passwd};
   $passwd = readFile($file);
   $server->{logger}->debug("Using passwd from '$file'");

 } elsif ( !$opt{passwd} ) {

   my $file = undef;
   my @search_path = ( map { file_name_is_absolute($_) ? $_ : catdir($server->wl_home(), $_) }
         split(/[\s,]+/, $Passwd_Search_Path) );

   foreach my $dir ( @search_path ) {
     $file = catfile($dir, $Passwd_File);
     last if ( -r $file );
   }

   $server->{logger}->debug("Using passwd from '$file'");
   $passwd = readFile($file);
   chomp($passwd) if ( $passwd );

 } else {

   $passwd = $opt{passwd};

 }

 if ( !$passwd and -t STDIN ) {
   print "Please enter system password: ";
   chomp($passwd = <STDIN>);
 }

 throw Error::Simple("Unable to determine password") if ( !$passwd );

 $server->{passwd} = $passwd;

 return ($passwd);
}


######################################################################
# config() - Returns the server configuration data structure.
#

sub config
{
 my $server = shift();

 if ( !$server->{config} ) {

   my $domain_config = new WDIG::Weblogic::Config($server->{domain}, wl_home=>$server->wl_home());
   $server->{config} = $domain_config->server($server->{name});

 }

 return ($server->{config}); 
}


######################################################################
# machine() - Returns the machine name for this server.
#

sub machine
{
 my $server = shift();
 return ($server->config()->{Machine}); 
}

######################################################################
# port() - Returns the listen port for this server.
#

sub port
{
 my $server = shift();
 return ($server->config()->{ListenPort}); 
}


######################################################################
# apps() - Returns list of apps targetted at this server.
#

sub apps
{
 my $server = shift();

 if ( !$server->{apps} ) {

   my $domain_config = new WDIG::Weblogic::Config($server->{domain}, wl_home=>$server->wl_home());
   $server->{apps} = $domain_config->server2app($server->{name});

 }

 return (wantarray ? @{$server->{apps}} : $server->{apps}); 
}


######################################################################
# get_mbean($type, $name) - Returns a config property.
#

sub get_mbean
{
 my $server = shift();
 my $mbean_type = shift();
 my $mbean_name = shift();

 my $config = new WDIG::Weblogic::Config($server->{domain}, wl_home=>$server->wl_home());

 return ($config->getElement($mbean_type, $mbean_name)); 
}


######################################################################
# get_runtime_mbean($type, $name) - Returns a runtime property.
#

sub get_runtime_mbean
{
 my $server = shift();
 my $mbean_type = shift();
 my $mbean_name = shift();
 my %opt = @_;

 %opt = ( %{$server->{opt}}, %opt, log_file=>0, pid_file=>0, show_output=>0 );

 my $config = new WDIG::Weblogic::Config::Runtime($server->{domain}, 
                                                  $server->{name}, 
                                                  %opt,
                                                  wl_home=>$server->wl_home(), 
                                                  passwd=>$server->passwd());

 return ($config->getElement($mbean_type, $mbean_name)); 
}


######################################################################
# jdbc_config($pool_name) - Returns the jdbc connection pool config.
# The mbean properties are augmented with elements more usefull for
# using as WDIG::DB::SqlInterpreter connection properties.
#

sub jdbc_config
{
 my $server = shift();
 my $pool_name = shift();
 my $pool_config = undef;

 try {

   $pool_config = $server->get_runtime_mbean('JDBCConnectionPoolConfig', $pool_name);

   for my $pool ( $pool_name ? $pool_config : values(%$pool_config) ) {
     my @props = map { split(/=/,ucfirst($_)) } split(/;/, $pool->{Properties});
     %$pool = ( @props, %$pool );
     $pool->{Server} = $1 if ( $pool->{URL} =~ /(\w+):\d+$/ );
     $pool->{Database} = $1 if ( $pool->{URL} =~ /(\w+)@/ );
   }

 } catch Error with {

   my $error = shift();
   throw $error if ( $error->text() !~ /JDBCConnectionPool.*not found/ );

 };

 return ($pool_config); 
}


1;
__DATA__

# Code for SSH remote end (pseudo Perl RPC!)
# Should be used with sprintf with 5 args:
# $server, $domain, Dump(%%opt), $method, $success_string

use strict;
use warnings;
use Error qw(:try);
use Log::Log4perl qw(:levels);
use WDIG::Weblogic::Server;

my $server_name = "%s";
my $domain_name = "%s";
my $opt = %s;
my $success = undef;

if ( $opt->{log_conf_file} ) {
  Log::Log4perl->init($opt->{log_conf_file});
} else {
  Log::Log4perl->init( {
    'log4perl.rootLogger' => 'INFO, screen',
    'log4perl.appender.screen' => 'Log::Log4perl::Appender::Screen',
    'log4perl.appender.screen.stderr' => 0,
    'log4perl.appender.screen.layout' => 'SimpleLayout'
  } );
  my $root_logger = Log::Log4perl->get_logger("");
  $root_logger->level($opt->{log_level}) if ( $opt->{log_level} );
}

$0 = "WDIG::Weblogic::Server [$domain_name:$server_name]";
$| = 1;

try {

  my $server = new WDIG::Weblogic::Server($server_name, $domain_name, %%$opt);
  $success = $server->%s();
  throw Error::Simple("The command did not succeed") if ( !$success );

} catch Error with {

  my $error = shift();
  print $error->text()."\n";
  exit(1);

};

print "$success";

exit(0);

__END__


