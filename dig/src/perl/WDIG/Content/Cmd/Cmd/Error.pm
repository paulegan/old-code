
# $Id$

=head1 NAME

WDIG::Content::Cmd::Error - Error sub-class for WDIG::Content::Cmd objects

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/20

=head1 SYNOPSIS

 use WDIG::Content::Cmd::Error qw(:try);

 try {

   throw WDIG::Content::Cmd::Error("my error");

 } catch Error with {

   my $error = shift();
   print STDERR "$error\n";

 };

 my $log = Log::Log4perl->get_logger("my.category");
 throw WDIG::Content::Cmd::Error("my error", $log);

=head1 DESCRIPTION

An Error class that logs to Log4perl and returns the error string
as XML.

=head1 EXPORTS

Nothing.

=head1 HISTORY

 $Log: Error.pm,v $
 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::Content::Cmd::Error;

use 5.006_001;
use strict;
use warnings;
use base qw(Error);
use Log::Log4perl qw(get_logger);

our @ISA;
our $VERSION = '1.0';


######################################################################
# new($text) - This constructor sets the text, file & line properties
# as normal but also creates a new logger for the calling package
# and logs the error text.
#

sub new
{
 my $error = shift();
 my $text  = shift() || "Unknown error";
 my $logger = shift();

 my %data = ();

 if ( $text =~ s/ at (\S+) line (\d+)(\.\n)?$//s ) {
   $data{-file} = $1;
   $data{-line} = $2;
 }

 $data{-text} = $text;

 $data{-logger} = $logger || get_logger(caller($Error::Depth));

 local $Error::Depth = $Error::Depth + 1;
 local $Log::Log4perl::caller_depth = $Error::Depth;

 $data{-logger}->error($text);

 return ($error->SUPER::new(%data));
}


######################################################################
# stringify() - The string overloader for this error class.
# Returns the error as XML.
#

sub stringify
{
 my $error = shift();
 my $text = $error->text();
 my $file = $error->file();
 my $line = $error->line();

 chomp($text);

 $text = "<error file=\"$file\" line=\"$line\">\n $text\n</error>";

 return ($text);
}


1;
__END__


