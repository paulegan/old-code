
# $Id$

=head1 NAME

WDIG::Content::Cmd - Parent command object used for performing operations on content

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/15

=head1 SYNOPSIS

 use WDIG::Content::Cmd;

 my $cmd = new WDIG::Content::Cmd($cmd_name);

 my $success = $cmd->exec($input);

 print $cmd->output();

=head1 DESCRIPTION

Cmd works as a factory object/abstract class which will return 
the appropriate sub-class for the specified command.  Each sub-class
implements the following methods.

=over 4

=item exec($input)

Executes the command.  The input will be read to determine the exact
operation to be performed and output will be set to the result.

=item output()

Returns the output value.

=item input($input)

Can be used to set the input value.

=back

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: Cmd.pm,v $
 Revision 1.3  2003/08/26 22:28:08  pegan
 Changed to more dependable Log4perl init check
 Added email_alert() method

 Revision 1.2  2003/07/02 00:48:27  pegan
 Fixed bug in sub-class load error check

 Revision 1.1  2003/06/30 02:48:04  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::Content::Cmd;

use 5.006_001;
use strict;
use warnings;

our @ISA;
our $VERSION = '1.0';

######################################################################

use Carp;
use XML::Simple qw(:strict);
use Log::Log4perl qw(get_logger);
use Mail::Sendmail qw(sendmail);
use WDIG::Content::Cmd::Error qw(:try);

my $Cmd_Aliases = {
  copy => 'push_drop'
};

my $Default_Log_Init = {
  'log4perl.logger.WDIG.Content.Cmd' => 'WARN, stdout',
  'log4perl.appender.stdout' => 'Log::Dispatch::Screen',
  'log4perl.appender.stdout.layout' => 'SimpleLayout'
};


######################################################################
# new($cmd,%options) - Loads the appropriate sub-class returns the
# value of _new().
#

sub new
{
 my $package = shift();

 if ( $package =~ /^WDIG::Content::Cmd::(.*)$/ ) {
   return ($package->_new($1,@_));
 }

 my $cmd_name = shift();
 my %opt = @_;

 throw WDIG::Content::Cmd::Error("No command name specified") if ( !$cmd_name );

 $cmd_name = lc($cmd_name);
 $cmd_name = $Cmd_Aliases->{$cmd_name} if ( exists($Cmd_Aliases->{$cmd_name}) );
 my $cmd = $package . '::' . $cmd_name;

 eval("require $cmd");

 throw WDIG::Content::Cmd::Error($@) if ( $@ and $@ !~ /Can.t locate WDIG.Content.Cmd/ );

 {
  no strict 'refs';
  throw WDIG::Content::Cmd::Error("Command not found: $cmd_name") unless ( @{"${cmd}::ISA"} );
 }

 return ($cmd->_new($cmd_name,%opt));
}


######################################################################
# _new($cmd,%options) - The object constructor returns an instance
# of the specified package.  This may be over-ridden by the sub-class.
#

sub _new
{
 my $package = shift();
 my $cmd_name = shift();
 my %opt = @_;

 my $self = {};

 $self->{name} = $cmd_name;
 $self->{input} = "";
 $self->{output} = "";

 bless($self, $package);

 $self->apply_defaults($opt{defaults}, \%opt) if ( $opt{defaults} );
 %{$self->{opt}} = %opt;

 if ( !Log::Log4perl->get_logger("")->has_appenders() ) {
   Log::Log4perl->init($self->{opt}->{log_conf_file} || $Default_Log_Init);
 }
 $self->{logger} = get_logger($package);

 return ($self);
}


######################################################################
# exec($input) - Implements the command.  This method must be
# provided by the sub-class and should set the output value to
# something apprpriate.
#

sub exec
{
 my $cmd = shift();
 my $input = shift() || $cmd->{input};

 throw WDIG::Content::Cmd::Error("exec() not implemented for ".ref($cmd)."!");

 return (1);
}


######################################################################
# output() - Returns the output value.
#

sub output
{
 my $cmd = shift();
 return ($cmd->{output});
}


######################################################################
# input($input) - Returns or sets the output value.
#

sub input
{
 my $cmd = shift();
 my $input = shift() || $cmd->{input};
 return ($cmd->{input} = $input);
}


######################################################################
# apply_defaults($defaults,\%opt) - Combines default options with
# the current values.
#

sub apply_defaults
{
 my $cmd = shift();
 my $defaults = shift();
 my $opt = shift();

 if ( $defaults and ref($defaults) ne 'HASH' ) {

   carp("Unable to parse defaults '$defaults': $@\n")
     if ( ! eval { $defaults = XMLin($defaults, ForceArray=>0, KeyAttr=>{}) } );

 }

 if ( $defaults and ref($defaults) eq 'HASH' ) {
   %$opt = ( %$defaults, %$opt );
   return (1);
 } else {
   return (undef);
 }
}


######################################################################
# email_alert(%alert) - Sends an email to us to alert us of 
# something.
#

sub email_alert
{
 my $cmd = shift();

 my %alert = ( %{$cmd->{opt}->{email_alert}}, @_ );

 my $success = sendmail(%alert)
       or throw WDIG::Content::Cmd::Error($Mail::Sendmail::error);
 
 return ($success);
}


1;
__END__


