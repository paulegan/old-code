
# $Id$

=head1 NAME

WDIG::HitBox - Some usefull functions related to HitBox

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/09/02

=head1 SYNOPSIS

 use WDIG::HitBox;

 remove_tag(canonpath($file));

=head1 DESCRIPTION

This package contains some usefull functions related to 
HitBox tagging.  These are mainly used by the other
WDIG::HitBox packages.  The following functions are
available:

=over 4

=item remove_tag($file)

Removes a HitBox tag from the specified file.  The function
will return 1 if the tag is found and is removed successfully.
Otherwise 0 is returned.

=item canonpath($path)

Returns the canonical version of the specified path.  This
function extends the File::Spec namesake to include the
tidying of 'xx/..' references.

=back

=head1 EXPORTS

 remove_tag
 canonpath

=head1 HISTORY

 $Log: HitBox.pm,v $
 Revision 1.1  2003/09/03 06:14:15  pegan
 Initial revision - based on code from hitbox v2.3


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::HitBox;

use 5.006_001;
use strict;
use warnings;
require Exporter;

our @ISA = qw(Exporter);
our @EXPORT = qw(remove_tag canonpath);
our @EXPORT_OK = qw();
our $VERSION = '1.0';

######################################################################

use File::Spec::Unix;
use WDIG::File::Slurp;

my $HitBox_Code_Begin = '<!--\s+BEGIN WEBSIDESTORY CODE\s+[^\-]*-->';
my $HitBox_Code_End   = '<!--\s+END WEBSIDESTORY CODE\s+-->[\r\n]+';


#########################################################################
# remove_tag($file) - removes the HitBox code from a file.
#
sub remove_tag
{
 my $file = shift();

 return(0) if ( !-e $file );

 my $contents = readFile($file);

 if ( $contents and $contents =~ /$HitBox_Code_Begin.*$HitBox_Code_End/so ) {
   $contents =~ s/$HitBox_Code_Begin.*$HitBox_Code_End//so;
   open(F, ">$file") or return(0);
   print F $contents;
   close(F);
   return(1);
 }

 return(0);
}


#########################################################################
# canonpath($path) - as from File::Spec::Unix but with the resolution
# of parent ".." references.
#
sub canonpath
{
 my $path = shift();

 $path = File::Spec::Unix->canonpath($path);

 while ($path =~ m|/\.\./|) {
   $path =~ s|[^/]*/\.\./||;
 }

 return($path);
}



1;
__END__
