
# $Id$

=head1 NAME

WDIG::Content::User::DB - A user database

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.egan@disney.com
 Date: 2001/11/27

=head1 SYNOPSIS

 use WDIG::Content::User::DB;

 my $user_db = new WDIG::Content::User::DB($db_file);

 $user_db->create($username,$passwd,$dir);

 my @user_list = $user_db->list();

 my $uid = $user_db->ID($username);

 $user_db->delete($username) if ( $user_db->exists($username) );

 my $user = new WDIG::Content::User();

 $user_db->add($user);
 
 my $user = $user_db->get($user->name());

 my $user = $user_db->search_by_url($url);
 my $user = $user_db->search_by_app($app_name, $domain_name, $url_map);

=head1 DESCRIPTION

Module for managing a user database in the form of a
standard password form, but with the GECOS field storing
email address & directory access.

=back

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: DB.pm,v $
 Revision 1.3  2003/06/30 02:41:19  pegan
 Added search and add methods
 convert ":" to "!" in passwd file
 Object now acts as a singleton

 Revision 1.2  2003/05/27 02:22:36  pegan
 Re-worked to new package standard

 Revision 1.1  2002/01/29 02:49:26  pegan
 Initial revision

=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::User::DB;

use 5.006_001;
use strict;
use warnings;

our @ISA = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use URI;
use Unix::PasswdFile;
use WDIG::Content::User;
use WDIG::Content::URL_Map;

my $User_DB;

my $Default_Passwd_File = '/share/lib/passwd';
my $Default_Home_Dir = '/';
my $Shell = '/dev/null';
my $Min_UID = 10001;
my $Def_GID = 10001;


######################################################################
# new($db_file) - verifies that the passwd file exists & is writable
#

sub new
{
 my $self = shift();
 my $file = shift() || $Default_Passwd_File;
 my %opt = @_;
 my $package = ref($self) || $self;

 return ($User_DB) if ( $User_DB and !$opt{force_reload} );
 
 croak("$file: not found") if ( !-w $file);

 my $user_db = { file=>$file };

 bless($user_db, $package);

 $User_DB = $user_db;

 return ($user_db);
}


######################################################################
# list() - returns a list of usernames
#

sub list
{
 my $user_db = shift();
 my $passwd_file = new Unix::PasswdFile($user_db->{'file'}, mode=>'r', locking=>'none');
 return ($passwd_file->users());
}


######################################################################
# exists($username) - returns true if the user exists
#

sub exists
{
 my $user_db = shift();
 my $passwd_file = new Unix::PasswdFile($user_db->{'file'}, mode=>'r', locking=>'none');
 return (defined($passwd_file->user(shift())));
}


######################################################################
# ID($username) - returns the ID of the specified user
#

sub ID
{
 my $user_db = shift();
 my $passwd_file = new Unix::PasswdFile($user_db->{'file'}, mode=>'r', locking=>'none');
 return ($passwd_file->uid(shift()));
}


######################################################################
# email($username,$address) - if address is given, sets the email address
# to that value, else returns the current value
#

sub email
{
 my $user_db = shift();
 my $user = shift();

 if ( @_ ) {

   my $passwd_file = new Unix::PasswdFile($user_db->{'file'});
   if ( $passwd_file->gecos($user) ) {
     my $dirs = ( split(";", $passwd_file->gecos($user)) )[1];
     my $gecos = shift().";".join(",", $dirs||"");
     $passwd_file->gecos($user, $gecos);
     return ($passwd_file->commit());
   }

 } else {

   my $passwd_file = new Unix::PasswdFile($user_db->{'file'}, mode=>'r', locking=>'none');
   if ( $passwd_file->gecos($user) ) {
     return ((split(";", $passwd_file->gecos($user)))[0] || '');
   }

 }

 return ('');
}


######################################################################
# dirs($username,@dirs) - if dirs are given, sets the dir list
# to that list, else returns the current list of directories
#

sub dirs
{
 my $user_db = shift();
 my $user = shift();

 if ( @_ ) {

   my $passwd_file = new Unix::PasswdFile($user_db->{'file'});
   if ( $passwd_file->gecos($user) ) {
     my $email = ( split(";", $passwd_file->gecos($user)) )[0];
     my $gecos = $email.";".join(",",@_);
     $gecos =~ s/:/!/g;
     # my $t = $passwd_file->home($user); $gecos =~ s/$t//g;
     $passwd_file->gecos($user, $gecos);
     return ($passwd_file->commit());
   }

 } else {

   my $passwd_file = new Unix::PasswdFile($user_db->{'file'}, mode=>'r', locking=>'none');
   if ( $passwd_file->gecos($user) ) {
     my $dirs = (split(";", $passwd_file->gecos($user)))[1];
     $dirs =~ s/!/:/g if ( $dirs );
     return ($dirs ? split(",", $dirs) : +());
   }

 }

 return (+());
}


######################################################################
# delete($username) - deletes the passwd entry for the specified user
#

sub delete
{
 my $user_db = shift();
 my $user = shift(); 
 my $passwd_file = new Unix::PasswdFile($user_db->{'file'});

 $passwd_file->delete($user);

 return ($passwd_file->commit());
}


######################################################################
# create($username,$passwd,$dir) - creates a new passwd entry for the
# user.
#

sub create
{
 my $user_db = shift();
 my $user = shift();
 my $passwd = shift();
 my $dir = shift() || $Default_Home_Dir;

 my $passwd_file = new Unix::PasswdFile($user_db->{'file'});
 my $uid = $passwd_file->maxuid() || $Min_UID;

 $passwd_file->user($user,
                    $passwd_file->encpass($passwd),
		    ($uid+1),
		    $Def_GID,
		    ";",
		    $dir,
		    $Shell );

 return ($passwd_file->commit());
}


######################################################################
# add($user) -
#

sub add
{
 my $user_db = shift();
 my $user = shift();

 if ( !$user_db->exists($user->name()) ) {
   return (undef) if ( !$user_db->create($user->name(), $user->passwd()) );
 }

 if ( $user->email() ) {
   $user_db->email($user->name(), $user->email());
 }

 if ( $user->url_list() ) {
   $user_db->dirs($user->name(), $user_db->dirs($user->name()), $user->url_list());
 }

 return ($user_db->get($user->name()));
}


######################################################################
# get($user_name) -
#

sub get
{
 my $user_db = shift();
 my $user_name = shift();

 if ( $user_db->exists($user_name) ) {
 
   my $user = new WDIG::Content::User($user_name,
                                      $user_db->email($user_name),
                                      'XXX',
                                      $user_db->dirs($user_name) );
   return ($user);
    

 } else {

   return (undef);

 }
}


######################################################################
# search_by_url($url) - 
#

sub search_by_url
{
 my $user_db = shift();
 my $url = shift();

 $url = new URI($url);
 croak("Invalid URL '$url'") unless ( $url and $url->scheme() and $url->scheme() =~ /^(ftp|http)$/ );
 my $host = $url->host();

 my @list = ();

 foreach my $user_name ( $user_db->list() ) {
   my $match = 0;
   foreach my $url ( $user_db->dirs($user_name) ) {
     $match = 1 if ( URI->new($url)->host() eq $host );
   }
   push(@list, $user_db->get($user_name)) if ( $match );
 }

 return ( @list );
}


######################################################################
# search_by_app($app_name,$domain_name,$url_map) - 
#

sub search_by_app
{
 my $user_db = shift();
 my $app_name = shift();
 my $domain_name = shift();
 my $url_map = shift();

 $url_map = new WDIG::Content::URL_Map($url_map);

 my ( $url ) = $url_map->search(app_name=>$app_name, domain_name=>$domain_name);

 if ( $url ) {
   return ( $user_db->search_by_url($url) );
 } else {
   return ( +() );
 }
}


1;
__END__


