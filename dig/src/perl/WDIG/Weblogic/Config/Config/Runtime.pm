
# $Id$

=head1 NAME

WDIG::Weblogic::Config::Runtime - A Config sub-class for retrieving runtime configuration elements

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/28

=head1 SYNOPSIS

 use WDIG::Weblogic::Config::Runtime;

 my $config = new WDIG::Weblogic::Config::Runtime($myDomain, $myServer);
 
 my $runtimeProp = $config->getElementProperty($type,$name,$property);

=head1 DESCRIPTION

This sub-class of WDIG::Weblogic::Config will query a running server
for the requested configuration detail rather than reading from the
XML file.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Runtime.pm,v $
 Revision 1.3  2003/08/27 02:43:53  pegan
 added options to run_admin_cmd

 Revision 1.2  2003/08/26 19:47:22  pegan
 mbean name now optional so that all object of a certain
 type can be returned

 Revision 1.1  2003/07/01 03:29:16  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::Weblogic::Config::Runtime;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::Weblogic::Config);

our @ISA;
our $VERSION = '1.0';

######################################################################

use Carp;
use WDIG::Weblogic::Cmd;


######################################################################
# new($domain|$configFile,$serverName,%options) - Config object 
# constructor.  Calls Config new() and sets server.
#

sub new
{
 my $self = shift();
 my $arg = shift();
 my $server_name = shift();
 my %opt = @_;
 my $package = ref($self) || $self;

 my $config = $package->SUPER::new($arg, %opt);

 croak("A server must be specified with a Runtime object") if ( !$server_name );

 my $server_config = $config->server($server_name);

 $config->{runtime}->{name} = $server_name;
 $config->{runtime}->{server} = $server_config;
 
 return ($config);
}


######################################################################
# getElement($type,$name,%options) - Returns the configuration of
# the specified configuration element.
#

sub getElement
{
 my $config = shift();
 my $type = shift();
 my $name = shift();
 my %opt = @_;

 %opt = ( %{$config->{opt}}, %opt );

 my $passwd = $opt{passwd};
 croak("Don't know what the password for the server is!") if ( !$passwd );

 ( my $base_type = $type ) =~ s/(Config|Runtime)$//;
 my $elements = $config->SUPER::getElement($base_type, $name, %opt);
 $elements = { $name => $elements } if ( $name );

 my $output = "";
 my $mbean_name = "$config->{name}:Location=$config->{runtime}->{name},Name=(\\w+),Type=$type";
 my $object = $name ? "-mbean $config->{name}:Location=$config->{runtime}->{name},Name=$name,Type=$type" : "-type $type";

 run_admin_cmd("get $object",
               $config->{runtime}->{server}->{Machine} || "localhost",
	       $config->{runtime}->{server}->{ListenPort},
	       $passwd,
	       output=>\$output,
               %opt)
    or croak($WDIG::Weblogic::Cmd::errstr);

 croak($output) if ( $output !~ /^{MBeanName=/ );

 while ( $output =~ /^{MBeanName="$mbean_name"(.*)}\s*$/mg ) {

   foreach my $property ( split(/[{}]+/,$2) ) {
     my ($name,$value) = split(/=/, $property, 2);
     $elements->{$1}->{$name} = $value if ( $name );
   }

 }

 return ($name ? $elements->{$name} : $elements);
}


1;
__END__


