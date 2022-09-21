
######################################################################
#
# ID:		$Id$
#
# Author:	paul.egan@disney.com
# 		2001/11/27
#
# Last update:	$Author$
# 		$Date$
#
# Description:	Module for managing a user database in the form of a
#		standard password form, but with the GECOS field storing
#		email address & directory access.
#
# Log:		$Log: UserDB.pm,v $
# Log:		Revision 1.1  2002/01/29 02:49:26  pegan
# Log:		Initial revision
# Log:
#
######################################################################

package UserDB;

use strict;
use warnings;
use Unix::PasswdFile;

my $DefaultPasswdFile = '/etc/passwd';
my $DefaultHomeDir = '/';
my $Shell = '/dev/null';
my $MinUID = 10001;
my $DefGID = 10001;


######################################################################
# Constructor - verifies that the passwd file exists & is writable
#
sub new {
 my $class = $_[0];
 my $filePath = $_[1] || $DefaultPasswdFile;
 my $self = {};
 bless($self,$class);

 return (0) if ( ! -w $filePath);

 $self->{'filePath'} = $filePath;

 return ($self);
}


######################################################################
# list() - returns a list of usernames
#
sub list
{
 my $self = shift();
 my $passwdFile = new Unix::PasswdFile($self->{'filePath'}, mode=>'r', locking=>'none');
 return ($passwdFile->users());
}


######################################################################
# exists(username) - returns true if the user exists
#
sub exists
{
 my $self = shift();
 my $passwdFile = new Unix::PasswdFile($self->{'filePath'}, mode=>'r', locking=>'none');
 return (defined($passwdFile->user(shift())));
}


######################################################################
# ID(username) - returns the ID of the specified user
#
sub ID
{
 my $self = shift();
 my $passwdFile = new Unix::PasswdFile($self->{'filePath'}, mode=>'r', locking=>'none');
 return ($passwdFile->uid(shift()));
}


######################################################################
# email(username,<address>) - if address is given, sets the email address
# to that value, else returns the current value
#
sub email
{
 my $self = shift();
 my $user = shift();
 if (@_){
   my $passwdFile = new Unix::PasswdFile($self->{'filePath'});
   if ($passwdFile->gecos($user)){
     my $dirs = (split(";",$passwdFile->gecos($user)))[1];
     my $gecos = shift().";".join(",",$dirs||"");
     $passwdFile->gecos($user,$gecos);
     return ($passwdFile->commit());
   }
 }else{
   my $passwdFile = new Unix::PasswdFile($self->{'filePath'}, mode=>'r', locking=>'none');
   if ($passwdFile->gecos($user)){
     return ((split(";",$passwdFile->gecos($user)))[0]);
   }
 }
 return (0);
}


######################################################################
# dirs(username,<dir>,..) - if dirs are given, sets the dir list
# to that list, else returns the current list of directories
#
sub dirs
{
 my $self = shift();
 my $user = shift();
 if (@_){
   my $passwdFile = new Unix::PasswdFile($self->{'filePath'});
   if ($passwdFile->gecos($user)){
     my $email = (split(";",$passwdFile->gecos($user)))[0];
     my $gecos = $email.";".join(",",@_);
     # my $t = $passwdFile->home($user); $gecos =~ s/$t//g;
     $passwdFile->gecos($user,$gecos);
     return ($passwdFile->commit());
   }
 }else{
   my $passwdFile = new Unix::PasswdFile($self->{'filePath'}, mode=>'r', locking=>'none');
   if ($passwdFile->gecos($user)){
     my $dirs = (split(";",$passwdFile->gecos($user)))[1];
     return ($dirs?split(",",$dirs):+());
   }
 }
 return (+());
}


######################################################################
# delete(username) - deletes the passwd entry for the specified user
#
sub delete
{
 my $self = $_[0];
 my $user = $_[1]; 
 my $passwdFile = new Unix::PasswdFile($self->{'filePath'});
 $passwdFile->delete($user);
 return ($passwdFile->commit());
}


######################################################################
# create(username,passwd,<dir>) - creates a new passwd entry for the
# user.
#
sub create
{
 my $self = $_[0];
 my $user = $_[1];
 my $passwd = $_[2];
 my $dir = $_[3] || $DefaultHomeDir;
 my $passwdFile = new Unix::PasswdFile($self->{'filePath'});
 my $uid = $passwdFile->maxuid() || $MinUID;
 $passwdFile->user($user,$passwdFile->encpass($passwd),($uid+1),$DefGID,";",$dir,$Shell);
 return ($passwdFile->commit());
}



1;
