
# $Id$

=head1 NAME

Bundle::WDIG::Deps - a bundle to install dependencies for the WDIG bundle

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/07/02

=head1 SYNOPSIS

 C<perl -MCPAN -e 'install Bundle::WDIG::Deps'>

=head1 CONTENTS

Bundle::CPAN

YAML

ExtUtils::ParseXS

Module::Info

Module::Build

URI 1.23

Bundle::LWP

Storable 0.47

Error 0.15

Time::HiRes 1.45

IPC::Run3 0.007

File::Basename

File::Rsync 0.30

File::Touch

File::ReadBackwards

Text::Wrap

Config::Properties

XML::Parser 2.31_1 - Expat patched version

XML::Parser::PerlSAX 0.07

XML::NamespaceSupport 1.08

XML::SAX 0.12

XML::SAX::Expat 0.35_1 - patched version

XML::DOM 1.42

XML::Simple 2.08

Params::Validate

Log::Dispatch 2.06

Log::Log4perl 0.31

Log::Dispatch::FileRotate 1.05

Mail::Sendmail 0.79

Unix::PasswdFile 0.06

RPC::XML 0.53

=head1 DESCRIPTION

Installing this bundle will cause CPAN to install all the modules
listed in the contents.

=head1 HISTORY

 $Log: Deps.pm,v $
 Revision 1.3  2003/08/26 22:49:40  pegan
 Added RPC::XML

 Revision 1.2  2003/07/14 15:44:50  pegan
 Added Config::Properties

 Revision 1.1  2003/07/03 14:43:12  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut

######################################################################

package Bundle::WDIG::Deps;

use 5.006_001;

our $VERSION = '1.0';

1;
__END__
