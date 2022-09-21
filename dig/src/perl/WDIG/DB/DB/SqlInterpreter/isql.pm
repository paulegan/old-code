
# $Id$

=head1 NAME

WDIG::DB::SqlInterpreter::isql - an interface to the isql program

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/08/25

=head1 SYNOPSIS

 use WDIG::DB::SqlInterpreter;

 my %db_props = (
   server => 'my_db_server',
   user => 'db_user',
   password => 'passwd'
 );

 my $isql = new WDIG::DB::SqlInterpreter('isql', %db_props);

 my $success = $isql->exec($sql_script);

 print $isql->output();

=head1 DESCRIPTION

An implementation of the SqlInterpreter api using the isql command
from Micro$oft.  If isql is not available on the local machine,
use an C<ssh> option in C<%db_props> which should evaluate to a
hash with at least C<cmd> and C<options> elements and optionally
a C<server> element. If there's no C<server> element then the
db server is used. e.g.:

 my $defaults = '<db server="db_server">\
                  <ssh cmd="ssh" options="-i my_key" server="win_box" />\
                 </db>';
 my $isql = new WDIG::DB::SqlInterpreter('isql', defaults=>$defaults);
 $isql->exec("select serverproperty('servername')", user=>'db_user', password=>'passwd');

This'll ssh to win_box and run isql there to connect to db_server.

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: isql.pm,v $
 Revision 1.1  2003/08/27 02:31:03  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::DB::SqlInterpreter::isql;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::DB::SqlInterpreter);

our @ISA;
our $VERSION = '1.0';

######################################################################

use Error qw(:try);
use WDIG::Run;

my @Default_Cmd = ( 'isql', '-b', '-n', '-h-1' );
                        # -b = set the exit status
                        # -n = don't diplay "N>"
                        # -h-1 = don't bother with headings

my $Default_Timeout = 60;


######################################################################
# exec($script) - executes the given sql script.
#

sub exec
{
 my $isql = shift();
 my $input = shift();
 my %opt = @_;
 my $success = undef;

 %opt = ( %{$isql->{opt}}, %opt );

 my $script = $isql->get_script($input);
 # lc the opt keys so that we can just dump WDIG::Weblogic::Server->jdbc_config() as options
 %opt = map { lc($_)=>$opt{$_} } keys(%opt);

 throw Error::Simple("No script to execute") if ( !$script );
 throw Error::Simple("No user specified") if ( !$opt{user} );
 throw Error::Simple("No password specified") if ( !$opt{password} );
 throw Error::Simple("No server specified") if ( !$opt{server} );

 my @cmd = ( $opt{cmd} ) || @Default_Cmd;
 @cmd = ( @cmd, '-S', $opt{server}, '-U', $opt{user}, '-P', $opt{password} );
 @cmd = ( @cmd, '-d', $opt{database} ) if ( $opt{database} );

 if ( $opt{ssh} and ref($opt{ssh}) eq 'HASH' ) {
   @cmd = ( $opt{ssh}->{cmd},
            split(/\s+/, $opt{ssh}->{options}),
            ( $opt{ssh}->{server} || $opt{server} ),
            join(" ",@cmd) );
 }

 my $output = "";

 $isql->{logger}->debug("Running ".join(" ",@cmd));

 $success = run_cmd(\@cmd, input => $script, 
                           output => \$output,
                           timeout => ( $opt{timeout} || $Default_Timeout ),
                           log_file => $opt{log_file},
                           log_input => 1 ) or
        throw Error::Simple($WDIG::Run::errstr);
 
 $isql->{output} = $output;

 return ($success);
}



1;
__END__


