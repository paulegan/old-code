
# $Id$

=head1 NAME

WDIG::DB::SqlInterpreter - SqlInterpreter factory

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/08/22

=head1 SYNOPSIS

 use WDIG::DB::SqlInterpreter;

 my %db_props = (
   server => 'my_db_server',
   user => 'db_user',
   password => 'passwd'
 );

 my $isql = new WDIG::DB::SqlInterpreter($interpreter, %db_props);

 my $success = $isql->exec($sql_script);

 print $isql->output();

=head1 DESCRIPTION

SqlInterpreter works as a factory object/abstract class which will 
return an interpreter implementation.  Each implementation will 
have the following methods.

=over 4

=item new([$type,] %db_props)

Creates a new interpreter instance of the specified type.  If
type is not given then the default type is used.  At minimum,
each implementation will respect the options C<server>, C<user>,
C<password> and C<database> in C<%db_props>.

=item exec($sql_script)

Executes the given SQL script.  The argument can be the name of
an sql file or a string containing the sql statements themselves.

=item output()

Returns the output from the sql script.

=item input($input)

Can be used to set the SQL script.

=back

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: SqlInterpreter.pm,v $
 Revision 1.1  2003/08/26 23:16:18  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################
# Standard package initialisation:

package WDIG::DB::SqlInterpreter;

use 5.006_001;
use strict;
use warnings;

our @ISA;
our $VERSION = '1.0';

######################################################################

use Error qw(:try);
use XML::Simple qw(:strict);
use Log::Log4perl;
use WDIG::File::Slurp;


my $Default_Log_Init = {
  'log4perl.logger.WDIG.DB.SqlInterpreter' => 'WARN, stdout',
  'log4perl.appender.stdout' => 'Log::Log4perl::Appender::Screen',
  'log4perl.appender.stdout.stderr' => 0,
  'log4perl.appender.stdout.layout' => 'SimpleLayout'
};

my $Default_Interpreter = 'isql';


######################################################################
# new($interpreter,%options) - Loads the appropriate sub-class returns the
# value of _new().
#

sub new
{
 my $package = shift();

 if ( $package =~ /^WDIG::DB::SqlInterpreter::(.*)$/ ) {
   return ($package->_new($1,@_));
 }

 my $impl_name = ($#_%2) ? $Default_Interpreter : shift();
 my %opt = @_;

 $impl_name = $opt{interpreter} if ( $opt{interpreter} );

 throw Error::Simple("No implementation specified") if ( !$impl_name );

 $impl_name = lc($impl_name);
 my $interpreter = $package . '::' . $impl_name;

 eval("require $interpreter");

 throw Error::Simple($@) if ( $@ and $@ !~ /Can.t locate WDIG.DB.SqlInterpreter/ );

 {
  no strict 'refs';
  throw Error::Simple("Implementation not found: $impl_name") unless ( @{"${interpreter}::ISA"} );
 }

 return ($interpreter->_new($impl_name,%opt));
}


######################################################################
# _new($interpreter,%options) - The object constructor returns an instance
# of the specified package.  This may be over-ridden by the sub-class.
#

sub _new
{
 my $package = shift();
 my $impl_name = shift();
 my %opt = @_;

 my $self = {};

 $self->{name} = $impl_name;
 $self->{input} = "";
 $self->{output} = "";

 bless($self, $package);

 $self->apply_defaults($opt{defaults}, \%opt) if ( $opt{defaults} );
 %{$self->{opt}} = %opt;

 $self->{logger} = Log::Log4perl->get_logger($package);

 return ($self);
}


######################################################################
# exec($input) - Implements the command.  This method must be
# provided by the sub-class and should set the output value to
# something apprpriate.
#

sub exec
{
 my $interpreter = shift();
 my $input = shift();

 throw Error::Simple("exec() not implemented for ".ref($interpreter)."!");

 my $script = get_script($input);

 return (1);
}


######################################################################
# output() - Returns the output value.
#

sub output
{
 my $interpreter = shift();
 return ($interpreter->{output});
}


######################################################################
# input($input) - Returns or sets the output value.
#

sub input
{
 my $interpreter = shift();
 my $input = shift() || $interpreter->{input};
 return ($interpreter->{input} = $input);
}


######################################################################
# apply_defaults($defaults,\%opt) - Combines default options with
# the current values.
#

sub apply_defaults
{
 my $interpreter = shift();
 my $defaults = shift();
 my $opt = shift();

 if ( ref($defaults) ne 'HASH' ) {

   $interpreter->{logger}->warn("Unable to parse defaults '$defaults': $@\n")
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
# get_script($input) - Determines if the input is a script or a file
# name and returns the actual script as appropriate.
#

sub get_script
{
 my $interpreter = shift();
 my $script = shift();

 if ( $script =~ /\// and -r $script ) {
   $interpreter->{logger}->debug("reading $script");
   $script = readFile($script);
 }
 
 return ($script);
}


1;
__END__


