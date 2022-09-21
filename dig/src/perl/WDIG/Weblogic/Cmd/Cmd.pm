
# $Id$

=head1 NAME

WDIG::Weblogic::Cmd - Functions for starting and stopping Weblogic servers.

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/04/27

=head1 SYNOPSIS

 use WDIG::Weblogic::Cmd;

 start_server($server,$domain,$passwd,log_file=>$stdout_log);

 ping_server($host,$port,$passwd));

 run_admin_cmd("VERSION",$host,$port,$passwd,success_pattern=>'WebLogic Server 6');

 stop_server($host,$port,$passwd,check_log_file=>$wl_log);

=head1 DESCRIPTION

These functions inplement a Perl interface to the Weblogic java
command line utilities and additionally verify, in a programatic manner,
if the commands succeed.

=over 4

=item start_server($server,$domain,$passwd,%opt)

Uses WDIG::Run to start a Weblogic server under a
watchdog process.  Will return undef if the server does not start.
The root directory for the server will be C<$opt{wd}> or C<$opt{wl_home}> 
or C<.>.  The following options are supported as well as the standard Run
options.

 classpath - override $ENV{CLASSPATH}
 bea_home - override $ENV{BEA_HOME}
 java_home - java_home/bin will be added to the path
 source_dir - specify the application source dir for the server
 java_options - java runtime options
 properties - set system properties

 timeout - this Run option is set to 180s by default
 success_pattern - this Run option is set to "Started WebLogic Admin Server"

=item stop_server($server_host,$server_port,$passwd,%opt)

Calls the SHUTDOWN function using run_admin_cmd().  Will return
true only if the server shuts down correctly. 
Accepts all run_admin_cmd() options as well as the following:

 check_log_file - specify a weblogic log to check and verify that
                  the server shutdown correctly
 log_success_pattern - defaults to "Shutdown has completed"
 log_timeout - time to wait for log_success_pattern (default 30s)

=item ping_server($server_host,$server_port,$passwd,%opt)

Calls the PING function using run_admin_cmd() and returns true if
the server responds.

=item run_admin_cmd($cmd,$server_host,$server_port,$passwd,%opt)

Runs the specified command (optionally with args) on the server using
the weblogic.Admin utility through WDIG::Run.  All
standard Run options are supported.  The standard Run success_pattern
and timeout defaults are used and error_pattern defaults to 
"Exception:".

=back

=head1 ERROR HANDLING

Each function will return true if successful and undef if not.  Consult
the $errstr global for detail on why the function failed.

=head1 EXPORTS

 start_server
 stop_server
 ping_server
 run_admin_cmd

=head1 HISTORY

 $Log: Cmd.pm,v $
 Revision 1.5  2003/07/14 17:12:06  pegan
 properties & java_options can now be data structures as well as strings
 added Log4perl support

 Revision 1.4  2003/07/01 03:54:41  pegan
 Slightly changed stop_server() log check logic

 Revision 1.3  2003/05/09 19:10:21  pegan
 Added ENV options to run_admin_cmd()
 Changed some constant names

 Revision 1.2  2003/05/07 13:31:35  pegan
 Removed options & properties stuff to Server package
 Changed name of Run package

 Revision 1.1  2003/05/02 01:25:24  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Weblogic::Cmd;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(start_server stop_server ping_server run_admin_cmd);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use File::Spec::Functions qw(catfile catdir);
use File::ReadBackwards;
use Log::Log4perl qw(:nowarn get_logger);
use WDIG::File::Slurp;
use WDIG::Run;

our $errstr;

my $Start_Cmd = 'java %s %s -Dweblogic.Domain=%s -Dweblogic.Name=%s weblogic.Server';
my $Start_Secs = 180;
my $Start_Cmd_Success_Pattern = 'Started WebLogic \w+ Server';
my $Start_Cmd_Error_Pattern = '<Emergency>';

my $Admin_Cmd = 'java weblogic.Admin -url t3://%s:%s -username system %s';
my $Admin_Cmd_Error_Pattern = 'Exception:';

my $Shutdown_Secs = 30;
my $Shutdown_Log_Success_Pattern = 'Shutdown has completed';
my $Shutdown_Cmd_Success_Pattern = 'Shutdown initiated';

my $Ping_Cmd_Success_Pattern = 'RTT';


######################################################################
# start_server($server,$domain,$passwd,%opt) - starts the specified
# weblogic server.  Returns true is successful, otherwise undef.
#

sub start_server
{
 my $server = shift();
 my $domain = shift();
 my $passwd = shift();
 my %opt = @_;
 my $success = undef;

 croak('Usage: start_server($server,$domain,$passwd,%opt)') if ( !$passwd );

 $opt{input} = $passwd;
 $ENV{PATH} = catdir($opt{java_home},'bin').':'.$ENV{PATH} if ( $opt{java_home} );
 $ENV{LD_LIBRARY_PATH} = $opt{ld_library_path} if ( $opt{ld_library_path} );
 $ENV{BEA_HOME} = $opt{bea_home} if ( $opt{bea_home} );
 $ENV{CLASSPATH} = $opt{classpath} if ( $opt{classpath} );
 umask($opt{umask}) if ( $opt{umask} );
 $opt{wl_home} = $ENV{WL_HOME} if ( !$opt{wl_home} );
 $opt{wd} = $opt{wl_home} || '.' if ( !$opt{wd} );
 $opt{success_pattern} = $Start_Cmd_Success_Pattern if ( !$opt{success_pattern} );
 $opt{error_pattern} = $Start_Cmd_Error_Pattern if ( !$opt{error_pattern} );
 $opt{timeout} = $Start_Secs if ( !$opt{timeout} );
 $opt{show_output} = 1 if ( !$opt{log_file} );
 $opt{program_name} = "watchdog [$domain:$server]" if ( !$opt{program_name} );

 my @cmd = split(/\s+/, sprintf($Start_Cmd, "", "", $domain, $server));
                                
 @cmd = grep { $_ } ( $cmd[0], 
          ( $opt{java_options} ? ( ref($opt{java_options}) eq 'ARRAY' ? @{$opt{java_options}} : split(/\s+/,$opt{java_options}) ) : 0 ), 
          ( $opt{properties} ? ( ref($opt{properties}) eq 'HASH' ? ( map { "-D$_=".$opt{properties}->{$_} } keys(%{$opt{properties}}) ) : split(/\s+/,$opt{java_options}) ) : 0 ), 
          @cmd[1..$#cmd] );

 get_logger()->debug("Env:\n", join("\n", map { "$_ = $ENV{$_}" } 'PATH', 'LD_LIBRARY_PATH','CLASSPATH') );
 get_logger()->debug("Cmd:\n", join(" ",@cmd));
 
 $success = run_cmd(\@cmd, %opt);
 
 $errstr = $WDIG::Run::errstr if ( !$success );
 
 return ($success);
}


######################################################################
# stop_server($server_host,$server_port,$passwd,%opt) - Calls the
# weblogic.Admin shutdown function.  Returns true if server shutdown
# successfully.
#

sub stop_server
{
 my ( $server_host, $server_port, $passwd, %opt ) = @_;
 my $success = undef;

 croak('Usage: stop_server($server_host,$server_port,$passwd,%opt)') if ( !$passwd );

 $opt{success_pattern} = $Shutdown_Cmd_Success_Pattern if ( !$opt{success_pattern} );

 $success = run_admin_cmd("shutdown", $server_host, $server_port, $passwd, %opt);

 $errstr = $WDIG::Run::errstr if ( !$success );


 if ( $success and $opt{check_log_file} ) {

   my $log = $opt{check_log_file};
   $log = catfile($opt{wd} || $opt{wl_home} || $ENV{WL_HOME} || '.', $log) if ( !-r $log );
   
   if ( -r $log ) {

     get_logger()->debug("Checking log file '$log'");

     my $secs_to_wait = $opt{log_timeout} || $Shutdown_Secs;
     my $success_pattern = $opt{log_success_pattern} || $Shutdown_Log_Success_Pattern;
     my $still_running = 1;

     while ( $still_running and $secs_to_wait>0 ) {
       my $log = new File::ReadBackwards($log);
       $still_running = ( $log->readline() !~ /$success_pattern/ );
       $log->close();
       $secs_to_wait--;
       sleep(1);
     }

     if ( $still_running ) {
       $errstr = "Server still running after ".($opt{log_timeout} || $Shutdown_Secs)." seconds";
       $success = undef;
     }

   } else {

     get_logger()->warn("Log file '$log' not found");

   }

 }

 return ($success);
}


######################################################################
# ping_server($server_host,$server_port,$passwd,%opt) - Calls the
# weblogic.Admin ping function.  Returns true if the ping was successful.
#

sub ping_server
{
 my ( $server_host, $server_port, $passwd, %opt ) = @_;
 
 croak('Usage: ping_server($server_host,$server_port,$passwd,%opt)') if ( !$passwd );

 $opt{success_pattern} = $Ping_Cmd_Success_Pattern if ( !$opt{success_pattern} );

 return (run_admin_cmd("ping", $server_host, $server_port, $passwd, %opt));
}


######################################################################
# run_admin_cmd($cmd,$server_host,$server_port,$passwd,%opt) - 
# Sends a command to a running Weblogic server using the 
# weblogic.Admin java utility.  Returns undef if unsuccessful.
#

sub run_admin_cmd
{
 my ( $cmd, $server_host, $server_port, $passwd, %opt ) = @_;
 my $success = undef;
 
 croak('Usage: run_admin_cmd($cmd,$server_host,$server_port,$passwd,%opt)') if ( !$passwd );

 $ENV{PATH} = catdir($opt{java_home},'bin').':'.$ENV{PATH} if ( $opt{java_home} );
 $ENV{LD_LIBRARY_PATH} = $opt{ld_library_path} if ( $opt{ld_library_path} );
 $ENV{BEA_HOME} = $opt{bea_home} if ( $opt{bea_home} );
 $ENV{CLASSPATH} = $opt{classpath} if ( $opt{classpath} );
 umask($opt{umask}) if ( $opt{umask} );
 $opt{error_pattern} = $Admin_Cmd_Error_Pattern if ( !$opt{error_pattern} );

 my @cmd = split(/\s+/, sprintf($Admin_Cmd, $server_host, $server_port, $cmd));

 get_logger()->debug("Cmd:\n", join(" ",@cmd));

 $success = run_cmd(\@cmd, input=>$passwd, %opt);

 print "\n" if ( $opt{show_output} ); # Normally dropped for some reason.

 $errstr = $WDIG::Run::errstr if ( !$success );

 return ($success);
}

1;
__END__


