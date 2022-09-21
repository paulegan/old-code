
# $Id$

=head1 NAME

WDIG::Content::Cmd::enable_url - Enables access for a user to a URL

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/27

=head1 SYNOPSIS

 use WDIG::Content::Cmd;

 my $cmd = new WDIG::Content::Cmd('enable_url');

 my $success = $cmd->exec($input);

 print $cmd->output();

=head1 DESCRIPTION

Enables access for a user to a URL

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: enable_url.pm,v $
 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd::enable_url;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Content::Cmd);

our @ISA;
our $VERSION = '1.0';

######################################################################

use WDIG::Content::Cmd::Error qw(:try);
use WDIG::Content::User;
use WDIG::Content::User::DB;


######################################################################
# exec($input) - 
#

sub exec
{
 my $cmd = shift();
 my $input = shift() || $cmd->{input};
 my %opt = @_;
 my $success = 1;

 %opt = ( %{$cmd->{opt}}, %opt );

 throw WDIG::Content::Cmd::Error("No input") if ( !$input );

 my $user;
 try {
   $user = new WDIG::Content::User($input);
 } catch Error with {
   my $error = shift();
   throw WDIG::Content::Cmd::Error("Unable to create user object: ".$error->text());
 };

 my $user_db = new WDIG::Content::User::DB($opt{user_db_file});
 
 $cmd->{output} = $user_db->add($user);

 return ($success);
}


1;
__END__


