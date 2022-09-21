
# $Id$

=head1 NAME

WDIG::DB::SqlInterpreter::utils_schema - an interface to the weblogic utils.Schema tool

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

 my $isql = new WDIG::DB::SqlInterpreter('utils_schema', %db_props);

 my $success = $isql->exec($sql_script);

 print $isql->output();

=head1 DESCRIPTION

An implementation of the SqlInterpreter api using the weblogic 
utils.Schema utility.

=head1 EXPORTS

Nothing

=head1 HISTORY

 $Log: utils_schema.pm,v $
 Revision 1.1  2003/08/27 02:31:03  pegan
 Initial revision


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

package WDIG::DB::SqlInterpreter::utils_schema;

use 5.006_001;
use strict;
use warnings;
use base qw(WDIG::DB::SqlInterpreter);

our @ISA;
our $VERSION = '1.0';

######################################################################

use File::Temp qw(tempfile);
use Error qw(:try);
use WDIG::Run;

my @Default_Cmd = ( 'java', 'utils.Schema' );
my $Default_URL_Prefix = 'jdbc:weblogic:mssqlserver4';
my $Default_URL_Port = 1433;
my $Default_Driver = 'weblogic.jdbc.mssqlserver4.Driver';

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
 %opt = map { lc($_)=>$opt{$_} } keys(%opt);

 my $script = $isql->get_script($input);

 throw Error::Simple("No script to execute") if ( !$script );
 throw Error::Simple("No user specified") if ( !$opt{user} );
 throw Error::Simple("No password specified") if ( !$opt{password} );
 throw Error::Simple("No server specified") if ( !$opt{server} );

 $script =~ s/\bgo\b/;/g; # change go's to semicolons
 $script =~ s/--.*$//m; # strip inline comments
 $script =~ s/\/\*.*?\*\///sg; # strip comments

 my ($fh, $file) = tempfile();
 $fh->print($script);
 $fh->close();

 my $jdbc_url = $opt{url} || ( "$Default_URL_Prefix:".($opt{database}?"$opt{database}@":"")."$opt{server}:$Default_URL_Port" );
 my $driver = $opt{driver} || $opt{drivername} || $Default_Driver;
 my @cmd = ( ( $opt{cmd} || @Default_Cmd ), $jdbc_url, $driver );
 @cmd = ( @cmd, '-u', $opt{user}, '-p', $opt{password}, '-verbose', $file );

 my $output = "";

 $isql->{logger}->debug("Running ".join(" ",@cmd));

 $success = run_cmd(\@cmd, output => \$output,
                           timeout => ( $opt{timeout} || $Default_Timeout ),
                           log_file => $opt{log_file},
                           log_input => 1 );

 unlink($file);

 $output =~ s/(password: ).*/$1********/; # we don't really want db passwds sent to developers

 throw Error::Simple($output) if ( $output =~ /Exception:/ );
 
 $isql->{output} = $output;

 return ($success);
}



1;
__END__


