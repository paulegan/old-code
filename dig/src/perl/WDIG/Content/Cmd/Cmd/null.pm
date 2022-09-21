
# $Id$

=head1 NAME

WDIG::Content::Cmd::null - the null command does nothing

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/06/30

=head1 SYNOPSIS

 use WDIG::Content::Cmd;

 my $cmd = new WDIG::Content::Cmd('null');

 my $success = $cmd->exec("");

 print $cmd->output();

=head1 DESCRIPTION

This command does nothing and can be used as a template for other
commands or as a simple test.

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: null.pm,v $
 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd::null;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Content::Cmd);

our @ISA;
our $VERSION = '1.0';

######################################################################

use WDIG::Content::Cmd::Error qw(:try);


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

 throw WDIG::Content::Cmd::Error("No input") if ( !defined($input) );

 try {

   throw WDIG::Content::Cmd::Error("expected null string") if ( $input ne "" );

 } catch Error with {

   my $error = shift();
   throw WDIG::Content::Cmd::Error("Invalid input: ".$error->text());

 };
 
 $cmd->{output} = "";

 return ($success);
}


1;
__END__


