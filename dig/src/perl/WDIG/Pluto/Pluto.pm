
# $Id$

=head1 NAME

WDIG::Pluto - Interface to the Pluto workflow app

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/08/14

=head1 SYNOPSIS

 use WDIG::Pluto;


=head1 DESCRIPTION


=head1 ERROR HANDLING


=head1 EXPORTS

=head1 HISTORY

 $Log: Pluto.pm,v $
 Revision 1.1  2003/08/26 22:53:58  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::Pluto;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw();
our @EXPORT_OK = qw(get_rpc_client);
our $VERSION = '1.0';

######################################################################

use Error;

my $rpc_client = undef;

######################################################################
# get_rpc_client() - returns a WDIG::Pluto::RpcClient.
#

sub get_rpc_client
{
 $rpc_client ||= new WDIG::Pluto::RpcClient(@_);
 return ($rpc_client);
}


1;
__END__
