
# $Id$

=head1 NAME

Bundle::WDIG - a bundle to install all WDIG modules

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/07/02

=head1 SYNOPSIS

 C<perl -MCPAN -e 'install Bundle::WDIG'>

=head1 CONTENTS

Bundle::WDIG::Deps 1.0

WDIG::File::Slurp 1.0

WDIG::File::Cmd 1.0

WDIG::Run 1.0

WDIG::Content::URL_Map 1.0

WDIG::Content::User 1.0

WDIG::Content::Drop 1.0

WDIG::Content::Cmd 1.0

WDIG::Weblogic::Cmd 1.0

WDIG::Weblogic::Config 1.0

WDIG::Weblogic::Server 1.0

WDIG::Weblogic::Application 1.0

WDIG::DB 1.0

WDIG::Pluto 1.0

=head1 DESCRIPTION

Installing this bundle will cause CPAN to install all the modules
listed in the contents.

=head1 HISTORY

 $Log: WDIG.pm,v $
 Revision 1.2  2003/08/26 22:52:07  pegan
 Added DB & Pluto

 Revision 1.1  2003/07/03 14:43:12  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut

######################################################################

package Bundle::WDIG;

use 5.006_001;

our $VERSION = '1.0';

1;
__END__
