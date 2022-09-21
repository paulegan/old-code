#!/usr/bin/perl -Tw

# $Id$

=head1 NAME

userAdmin.pl - A wrapper script for plutoShell called by pluto

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/26

=head1 SYNOPSIS

 userAdmin.pl < cmds

=head1 DESCRIPTION

This script was superceded by plutoShell but is still called by
pluto, pending an update to pluto to meet the latest interface
specification.  See the plutoShell documentation for information
on valid commands.  plutoShell STDOUT & STDERR are passed
directly to pluto.  The exit status will be the same as for
plutoShell.

=head1 HISTORY

 $Log: userAdmin.pl,v $
 Revision 2.2  2003/08/27 21:44:52  pegan
 changed content_cmd to squirt

 Revision 2.1  2003/07/16 15:20:19  pegan
 disabled content_cmd call

 Revision 2.0  2003/07/16 14:49:10  pegan
 Reformatted to new style
 Added call to content_cmd


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

use 5.006_001;
use strict;
use warnings;

use lib('/share/src/scripts/test_lib','/share/lib/perl5/5.6.1','/share/lib/perl5/site_perl');
use IPC::Open3;
use IO::Handle;
use URI;
use WDIG::Run;
use WDIG::Content::User;

$ENV{'PATH'} = '/bin:/usr/bin';

my @Cmd_PlutoShell = ( '/share/src/scripts/plutoShell' );
my @Cmd_Squirt = ( '/share/bin/squirt', 'push_drop' );
my $Squirt_Log = '/share/var/squirt.stdout';
my $Delete_Queue_File = '/share/var/delete.queue';

my $Cmd_Map = {
  'ADD' => 'enable_url',
  'REMOVE' => 'disable_url',
};

my $slew = 1800; # 30 min slew compensation


######################################################################
# start plutoShell & send commands

my $input = new IO::Handle();
my @input = ();

STDOUT->autoflush(1);

my $pid = open3($input, ">&STDOUT", ">&STDERR", @Cmd_PlutoShell);

while (<STDIN>){
  if ($_ =~ /LIST ([\s\S]+) ([\d]+)/) {
    my $adjTime = $2 - $slew;
    $_ =~ s/LIST ([\s\S]+) ([\d]+)/LIST $1 $adjTime/;
  }
  print $input $_ if ( $_ !~ /^DELETE/ );
  push(@input, $_);
}
$input->close();

waitpid($pid,0);
my $exit_status = $? >> 8;


######################################################################
# Dump delete commands to delete queue

my @delete_cmds = grep(/^DELETE/, @input);
if ( @delete_cmds ) {
  open(DQ, '>>', $Delete_Queue_File) or warn($!);
  print DQ  @delete_cmds;
  close(DQ);
}


######################################################################
# exec squirt in the background

@input = (); # temporarily disable squirt

foreach my $line ( @input ) {

 my ($cmd, $url, @args) = split(/\s+/,$line);
 
 if ( $cmd = $Cmd_Map->{$cmd} ) {

   my @squirt = ( @Cmd_Squirt, $cmd );

   $url = new URI($url);
   my $user = new WDIG::Content::User(
               $url->user(),
               ( $args[0] and URI->new($args[0])->scheme() eq 'mailto' ) ? URI->new($args[0])->to() : undef,
               $url->password(),
	       $url->as_string()
	      );

   my $success = run_cmd(\@squirt, 
 		       input=>$user, 
 		       timeout=>-1, # -1 = return immediately
                       background=>1,
 		       log_file=>$Squirt_Log);

   warn($WDIG::Run::errstr) if ( !$success );

 }

}

exit($exit_status);

__END__
