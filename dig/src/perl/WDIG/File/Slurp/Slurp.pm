
# $Id$

=head1 NAME

WDIG::File::Slurp - functions for reading and writing files directly from variables.

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/04/16

=head1 SYNOPSIS
 
 use WDIG::File::Slurp;
 
 $fileContents = readFile($fileName);
 writeFile($fileName,$fileContents);
 @contentByLine = readFile($fileName);

=head1 DESCRIPTION

Simple functions for reading and writing files directly from variables.
readFile() returns a scalar or an array depending on the context and
undef on error.  writeFile() returns 1 on success and undef on error.

=head1 EXPORTS

 readFile
 writeFile
 
=head1 HISTORY

 $Log: Slurp.pm,v $
 Revision 1.2  2003/06/30 03:03:02  pegan
 Check first that the file arg is defined

 Revision 1.1  2003/04/17 15:01:59  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut

######################################################################

package WDIG::File::Slurp;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(readFile writeFile);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################
# readFile($fileName)

sub readFile
{
 my $file = shift();
 my @contents = ();
 local $/ = wantarray ? $/ : undef;;

 if (defined($file) && -e $file && open(F, "<$file")) {
   @contents = <F>;
   close(F);
 }
 
 return (wantarray ? @contents : $contents[0]);
}


######################################################################
# writeFile($fileName,@data)

sub writeFile
{
 my $file = shift();
 my @contents = @_;

 open(F, ">$file") || return (undef);
 print F @contents;
 close(F);
 
 return (1);
}

1;
__END__

