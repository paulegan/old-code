
# $Id$

=head1 NAME

WDIG::Run - Generic function for running a command and verifying its output

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/04/27

=head1 SYNOPSIS

 use WDIG::Run;

 my $success = run_cmd(\@cmd,
 	success_pattern=>'my success string',
 	timeout=>60,
 	wd=>'/some/dir',
 	input=>'some\ninput',
 	show_output=>1,
 	log_file=>'my_cmd.log',
 	pid_file=>'watchdog.pid',
 	program_name=>'watchdog',
 	error_pattern=>'[Er]rror');

=head1 DESCRIPTION

The run_cmd function provides a generic mechanism for running a command,
verifying & logging it's output and handling signals.  The function
runs in two seperate modes.  If a background or success_pattern option
is specified then the command is run in background mode otherwise
normal mode is used.

NORMAL MODE

The functions will fork & exec the command, the input is send
to the process' stdin and stdout/stderr is copied to the log file
or output variable.  If an error is caught, the process is
killed.  The function will return successfully if the process
completes with no error and the exit status is 0.

BACKGROUND MODE

A watchdog process will be spawned and the calling script will block
waiting for feedback from this script.  The watchdog will fork & exec
the command and check the output for a success string; if no output is
received within a certain time or if an error is caught, the process is
killed.  If the success  string is received the watchdog detaches and
the function returns.  

All output from the command is sent to a log file.  When detached the
watchdog process will accept certain signals - a TERM signal will cause
the watchdog to kill the command and a HUP signal will re-open the log
file (useful for log rotation).

OPTIONS

=over 4

=item background

If set to true the command will be run in background mode.  Default
is true if success_pattern is specified, otherwise false.

=item success_pattern

Defines the regex that determines whether the command has been 
successful.  Default: '.' i.e. any output is a success.

=item timeout

Sets the timeout (in seconds) for waiting for command output.  
Default: 10.  A negative timeout will cause the function to
not wait.

=item error_pattern

If defined, the command will be killed if the output matches this
regex.  Default: undef.

=item log_file

Specifies the log file to send command stdout & stderr to.  If non
is specified then the output is discarded.  Default: undef.

=item output

Should reference a scalar to which the command output will be
appended.

=item show_output

Enables the echoing of the command output to stdout up till the success
string is read or the command is killed.  Default: 0.

=item input

If specified, this string is sent to the stdin of the command.
Default: undef.

=item log_input

Will log the input as well as the command output.

=item wd

If set, a chdir to the specified value is done before executing the
command.  Default: undef.

=item pid_file

Will write the PID of the watchdog process to the specified file.
Default: undef.

=item program_name

Sets the name of the backgrounded watchdog process. Default: undef.

=back

=head1 ERROR HANDLING

Will return undef on error.  Consult the $errstr global for the
reason.

Informational messages will be sent to the log file using the 
format specified by the $Log_Date_Format global.

=head1 EXPORTS

run_cmd

=head1 HISTORY

 $Log: Run.pm,v $
 Revision 1.9  2003/08/26 22:01:44  pegan
 output now used as errstr for foreground non-zero exit

 Revision 1.8  2003/07/14 17:09:54  pegan
 Cmd input now uses select

 Revision 1.7  2003/07/03 00:53:54  pegan
 Added sleep() before watchdog exec to handle but with immediate background

 Revision 1.6  2003/06/30 03:06:39  pegan
 Now has two modes of operation: normal and background

 Revision 1.5  2003/05/28 22:33:45  pegan
 std streams now attached to /dev/null and not closed
 all others FDs are closed after pseudo exec

 Revision 1.4  2003/05/09 19:03:18  pegan
 Changed package name
 watchdog loop now correctly catches signals
 Added log_input option
 Updated open3 error pattern

 Revision 1.3  2003/05/02 01:32:44  pegan
 Fixed issue with older children interfering with watchdog feedback

 Revision 1.2  2003/04/30 18:51:32  pegan
 Changed to fork a watchdog process up front rather than fork after
 success.  This means the cmd is truely a child of the watchdog.
 Used IPC::Msg to return $errstr to the parent.
 Added some extra comments and changed some identifier names.

 Revision 1.1  2003/04/30 00:20:35  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Run;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(run_cmd);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use IPC::Open3;
use IPC::SysV qw(IPC_CREAT S_IRWXU MSG_NOERROR);
use IPC::Msg;
use IO::Handle;
use IO::File;
use IO::Select;
use POSIX qw(:sys_wait_h setsid strftime sysconf _SC_OPEN_MAX);
use File::Spec::Functions qw(devnull);
use WDIG::File::Slurp;

our $errstr = "";			# Set when a function returns undef

my $Default_Success_Timeout = 10;	# Default timeout on cmd stdout select

my $Open3_Error = '(^open3: |IPC.Open3\.pm)';	# Pattern to catch exec errors from open3

					# Format strings used by log_message
my $Log_Format = "<%s> <%s> <watchdog> <%s>\n"; # Date, level, message
my $Log_Date_Format = "%d-%b-%g %H:%M:%S %Z";

my $IPC_Message_Type = 1;		# Message type ID
my $IPC_Message_Max_Size = 1024;	# Max size of message buffer

my $dev_null = undef;
my $is_watchdog = undef;

$SIG{'PIPE'} = 'IGNORE';
# $SIG{'CHLD'} = 'IGNORE';


######################################################################
# run_cmd(\@cmd,%opt) - 
#

sub run_cmd
{
 my $cmd = shift();
 my %opt = @_;

 $errstr = "";

 $opt{success_pattern} = '.' if ( $opt{background} and !$opt{success_pattern} );

 if ( $opt{success_pattern} ) {

   croak("output option cannot be used in background mode") if ( $opt{output} );
   $is_watchdog = 1;
   return (fork_watchdog($cmd, %opt));

 } else {

   $is_watchdog = 0;
   return (exec_cmd($cmd, %opt));

 }

}


######################################################################
# fork_watchdog(\@cmd,%opt) - Forks a watchdog process to start the cmd
# and waits for feedback.  
# Returns 1 on success otherwise undef and sets $errstr global.
#

sub fork_watchdog
{
 my $cmd = shift();
 my %opt = @_;

			# msg_queue is used to get errstr from watchdog
 my $msg_queue = new IPC::Msg($$, IPC_CREAT | S_IRWXU);

 my $watchdog_pid = fork();			# fork

 if ( !defined($watchdog_pid) ) {		# catch failure
   $msg_queue->remove();
   $errstr = "Unable to fork: $!";
   return (undef);
 }

 if ( !$watchdog_pid ) {			# exec watchdog
   setsid();
   # close_all_fds($^F);
   sleep(1); # sleep for bit so the parent has time to sort signal order
   my $result = exec_cmd($cmd,%opt);
   $msg_queue->snd($IPC_Message_Type,$errstr) if ( $errstr );
   sleep(1); # sleep again for the same reason
   exit($result ? 0 : 1);
 }

 my $buffer = undef;                            # this is now the parent
 my $signal = 0;
 local $SIG{'USR1'} = sub { $signal=shift(); };
 local $SIG{'CHLD'} = sub { $signal=shift(); };

 do {						# Wait for watchdog, ignoring older children
   while ( !$signal && !defined($buffer) ) {
     $msg_queue->rcv($buffer, $IPC_Message_Max_Size, $IPC_Message_Type, MSG_NOERROR);
   }
 } while ( $signal eq 'CHLD' and waitpid($watchdog_pid, WNOHANG) != $watchdog_pid );
 $msg_queue->remove();

						# watchdog backgrounded
 if ( $signal eq 'USR1' or ( defined($buffer) and $buffer eq "" ) ) {
   return (1);
 }

 if ( $buffer ) {				# error from watchdog
   $errstr = $buffer;
 } else {
   $errstr = "Got SIG$signal\n";		# watchdog exited abnormally
 }

 waitpid($watchdog_pid, 0);

 return (undef);
}


######################################################################
# exec_cmd(\@cmd,%opt) - Forks & execs cmd with open3 and 
# verifies that it start up OK.  The cmd is killed on error.
# Returns 1 on success otherwise undef and sets $errstr global.
#

sub exec_cmd
{
 my $cmd = shift();
 my %opt = @_;

 chdir($opt{wd}) if ( $opt{wd} );

 my $success_pattern = $opt{success_pattern}; # not checked when undef 
 my $error_pattern = $opt{error_pattern}; # not checked when undef 
 my $timeout = $opt{timeout} || $Default_Success_Timeout; # Time to wait for success pattern
 $timeout = 0 if ( $timeout < 0 and !$is_watchdog );

 if ( $is_watchdog ) {				# re-open STDIN from /dev/null
   $dev_null = new IO::File(devnull(),'+<') or die($!);
   STDIN->fdopen($dev_null->fileno(),'<');
 }

 my $in = new IO::Handle();
 my $out = new IO::Handle();
						# open log file or /dev/null
 my $log = ( new IO::File($opt{log_file} || devnull(),'>>') );
 if ( !$log ) { $errstr = "Unable to open log $opt{log_file}: $!"; return (undef); }

 my $output = undef;
 if ( $opt{output} ) {
   croak("output option must be a scalar reference") if ( ref($opt{output}) ne 'SCALAR' );
   $output = $opt{output};
 }

 my $cmd_pid = open3($in,$out,$out,@$cmd);	# fork & exec cmd with 2>&1

 $log->syswrite(log_message("Started ($cmd_pid) ".join(" ",@$cmd)));


 if ( $opt{input} ) {			        # send input & close cmd stdin
   my $sel = new IO::Select($in);
   my @ready = $sel->can_write($timeout);
   if ( @ready ) {
     $in->syswrite($opt{input}) or carp("Write error: $!");
     $log->syswrite(log_message("Sent input: $opt{input}")) if ( $opt{log_input} );
   } else {
     $log->syswrite(log_message("Timeout on sending input: $!","Alert"));
   }
 }
 $in->close();

 STDOUT->autoflush(1) if ( $opt{show_output} );

 my $buffer_size = ($out->stat())[11] || 4096;
 my $buffer = " " x $buffer_size;
 my $success = undef;
 my $error = undef;

 # loop while no success or error and there's output within the timout
 my $sel = new IO::Select($out);
 while ( $timeout>=0 and !$success and !$error and $sel->handles() and
 	 my @ready = $sel->can_read($timeout) ) {

   foreach my $fh ( @ready ) {
     my $count = $fh->sysread($buffer, $buffer_size);
     if ( $count ) { 
       $log->syswrite($buffer);
       STDOUT->print("$buffer") if ( $opt{show_output} );
       $$output .= $buffer if ( $opt{output} );
       if ( $buffer =~ /$Open3_Error/m or ( $error_pattern and $buffer =~ /$error_pattern/m ) ) {
         $error = 1;
       } elsif ( $success_pattern and $buffer =~ /$success_pattern/m ) {
         $success = 1;
       }
     } else { # EOF
       $fh->close();
       $sel->remove($fh);
     }
   }

 }

						# if we got success str
 if ( $success or $timeout<0 ) {		# detach from parent

   send_to_background($cmd_pid,$out,$log,%opt) || return (undef);

 } else {					# else catch error & kill cmd

   if ( $error ) {
     $log->syswrite(log_message("Caught error string: $buffer","Alert"));
   } elsif ( !$sel->handles() ) {
     $log->syswrite(log_message("EOF on STDOUT","Alert"));
   } else {
     $log->syswrite(log_message("Timed out waiting for success: $!","Alert"));
   }

   kill('KILL',$cmd_pid);

 }

 if ( waitpid($cmd_pid, 0) and $? != -1 ) {
   my $caught_signal = $? & 127;
   my $error_code = $? >> 8;
   $log->syswrite(log_message("Signal: $caught_signal Exit code: $error_code"));
   $success = 1 if ( !$error and !$caught_signal and !$error_code );
 }

 $log->close();

 $errstr = $$output if ( $output and $$output and !$success and !$is_watchdog );

 return ($success);
}


######################################################################
# send_to_background($pid,$output,$log,%opt) - goes to background and logs
# command stdout.  Returns 1 on success otherwise undef and sets
# $errstr global.

sub send_to_background
{
 my $cmd_pid = shift();
 my $out = shift();
 my $log = shift();
 my %opt = @_;
 my $buffer_size = ($out->stat())[11] || 4096;
 my $buffer = " " x $buffer_size;

 if ( !$cmd_pid ) { $errstr = "daemonise: no pid"; return (undef); }
 if ( !$out ) { $errstr = "daemonise: no output handle"; return (undef); }
 if ( !$log ) { $errstr = "daemonise: no log handle"; return (undef); }

 $0 = $opt{program_name} if ( $opt{program_name} );

 STDOUT->fdopen($dev_null->fileno(),'>');
 STDERR->fdopen($dev_null->fileno(),'>');

 if ( $opt{msg_queue} ) {	# Notify parent that we've backgrounded
   $opt{msg_queue}->snd($IPC_Message_Type,"");
 } else {
   kill('USR1',getppid());
 }

 $log->syswrite(log_message("Successfully backgrounded ($$)"));
 writeFile($opt{pid_file},$$) if ( $opt{pid_file} );

 my $signal = 0;		# Trap signals
 local $SIG{'HUP'} = sub { $signal=shift(); };
 local $SIG{'TERM'} = sub { $signal=shift(); };
 my $sel = new IO::Select($out);

 while ( $out->opened() ) {

   while ( !$signal and my ($fh) = $sel->can_read(5) ) {
     $fh->sysread($buffer, $buffer_size) or $fh->close();
     $log->syswrite($buffer);  # Write everything to stdout log
   }

   if ( $signal eq 'HUP' and $opt{log_file} ) {	# Re-open log on SIGHUP
     $log->syswrite(log_message("Caught SIGHUP - reopening log","Alert"));
     $log->close();
     $log->fdopen(new IO::File($opt{log_file},'>>'),'>>') || die($!);
     $signal = 0;
   }
   
   if ( $signal eq 'TERM' ) {			# Kill off app
     $log->syswrite(log_message("Caught SIGTERM - going postal","Alert"));
     $out->close();
     kill('KILL',$cmd_pid);
   }

 }
 $out->close();

 $log->syswrite(log_message("EOF on STDOUT","Alert")) if ( !$signal );

 return (1);
}


######################################################################
# close_all_fds() - close all open file descriptors with an ID
# greater than the specified ID.
#

sub close_all_fds
{
 my $first = shift() || 0;
 my $max_fd = sysconf(_SC_OPEN_MAX) - 1;

 for (my $fd = $first+1; $fd<=$max_fd; $fd++) {
   POSIX::close($fd);
 }

 return ($max_fd-$first);
}


######################################################################
# log_message($message,$log_level) - format a log message in a nice
# manner.  Will also set $errstr global.
#

sub log_message
{
 my $message = shift();
 my $level = shift() || 'Notice';
 my $date = strftime($Log_Date_Format, localtime());
 $errstr = $message if ( $level eq 'Alert' );
 return (sprintf($Log_Format,$date,$level,$message));
}


1;
__END__
