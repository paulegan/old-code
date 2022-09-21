
# $Id$

=head1 NAME

WDIG::Weblogic::Config::Default - Function for adding default attributes to XML elements

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/04/24

=head1 SYNOPSIS

 use WDIG::Weblogic::Config::Default;
 my $server_XML_with_defaults = setDefaults("<Server/>");

=head1 DESCRIPTION

This module contains only one function - setDefaults().  This function
applies an XSLT to the provided XML to add default values as defined
in a XML Schema.

Both the XSLT and XML Schema files are searched for and read from @INC.
You can override the Schema file location by using the C<schema_file>
option to setDefaults().

=head1 EXPORTS

setDefaults

=head1 HISTORY

 $Log: Default.pm,v $
 Revision 1.2  2003/07/01 03:35:33  pegan
 Changed package name used in findFiles() to be static

 Revision 1.1  2003/05/09 19:14:48  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Weblogic::Config::Default;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(setDefaults);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use Carp;
use XML::LibXSLT;
use XML::LibXML;
use File::Spec::Functions('catfile','rel2abs');

my $XSLT_File = 'defaults.xsl';
my $Schema_File = 'config.xsd';

my $Stylesheet = undef; # Used for caching the parsed XSLT

findFiles();


######################################################################
# Looks for the XSL & Schema files in @INC

sub findFiles
{

 my $package = "WDIG::Weblogic::Config::Default";
 my $package_path = catfile(split('::',$package));
 my @include_dirs = ( map(catfile($_,$package_path),@INC), @INC );

 foreach my $dir ( @include_dirs ) {
   my $file = rel2abs(catfile($dir,$XSLT_File));
   if ( -e $file ) {
     $XSLT_File = $file;
     last;
   }
 }

 foreach my $dir ( @include_dirs ) {
   my $file = rel2abs(catfile($dir,$Schema_File));
   if ( -e $file ) {
     $Schema_File = $file;
     last;
   }
 }

 return (1);
}

######################################################################
# setDefaults($xml,%options) - Run the XSLT over a weblogic config
# xml document. Returns the modified doc.
#

sub setDefaults
{
 my $xml = shift();
 my %opt = @_;

 $opt{schema_file} = $Schema_File if ( !$opt{schema_file} );

 croak("No XML") if ( !$xml );
 croak("Config schema not found in \@INC") if ( !-r $opt{schema_file} );
 croak("Defaults XSLT not found in \@INC") if ( !-r $XSLT_File );

 $opt{schema_file} = "'$opt{schema_file}'";

 my $parser = XML::LibXML->new();
 my $xslt = XML::LibXSLT->new();
 
 if ( !defined($Stylesheet) ) {
   my $xsl = $parser->parse_file($XSLT_File);
   $Stylesheet = $xslt->parse_stylesheet($xsl);
 }

 my $doc = $parser->parse_string($xml);
 my $result = $Stylesheet->transform($doc,%opt);

 return ($Stylesheet->output_string($result));
}


1;
__END__


