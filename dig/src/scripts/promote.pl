#!/usr/bin/perl -Tw

# $Id$

=head1 NAME

promote.pl - A wrapper script for squirt called by pluto

=head1 AUTHOR

 $Author$
 $Date$

 Initial Author: paul.x.egan.-nd@email.disney.com
 Date: 2003/05/14

=head1 SYNOPSIS

 promote.pl source dest < file_list

=head1 DESCRIPTION

This script was superceded by plutoShell and in turn squirt but 
is still called by pluto, pending an update to pluto to meet 
the latest interface specification.  The command line arguments 
and input are used to form a WDIG::Content::Drop object which
is given as the input to the squirt push_drop command.  squirt
is run in the background after the drop is validated.

=head1 HISTORY

 $Log: promote.pl,v $
 Revision 3.0  2003/09/03 10:15:41  pegan
 Removed plutoShell call
 Added plutoShell style validation status codes

 Revision 2.2  2003/08/27 21:41:54  pegan
 Changed log_file variable name

 Revision 2.1  2003/08/27 00:24:22  pegan
 Added call to squirt

 Revision 2.0  2003/05/14 22:26:38  pegan
 Imported from old quick hack


=head1 COPYRIGHT

(C) Walt Disney Internet Group.  All rights reserved.

=cut


######################################################################

use 5.006_001;
use strict;
use warnings;

use lib('/share/src/perl/test_lib','/share/lib/perl5/5.6.1','/share/lib/perl5/site_perl');
use Error qw(:try);
use WDIG::Run;
use WDIG::Content::Drop;
use WDIG::Content::URL_Map;

$ENV{'PATH'} = '/bin:/usr/bin';

my @Cmd_Squirt = ( '/share/bin/squirt', 'push_drop' );
my $Squirt_Log = '/share/var/squirt.stdout';


######################################################################
# Read command line & input:

my $source = $1 if ($ARGV[0] and $ARGV[0] =~ /([\S]+)/);
my $dest = $1 if ($ARGV[1] and $ARGV[1] =~ /([\S]+)/);

die("500 Internal Error\nNo command line arguments found\n") if (!$source or !$dest);

my @file_list = ();
push(@file_list, $_) while (<STDIN>);
chomp(@file_list);


######################################################################
# Create drop:

my $drop;
my $invalid_files = {};
try {
  local $SIG{__WARN__} = sub { return WDIG::Content::Drop::file_list_warn_handler($invalid_files,@_); };
  $drop = new WDIG::Content::Drop($source, $dest, @file_list);
} catch Error with {
  my $error = shift();
  for ( $error->text() ) {
    /source/ and die("451 Source URL is invalid\n".$error->text()."\n");
    /dest/ and die("452 Destination URL is invalid\n".$error->text()."\n");
  } 
  die("500 Internal Error\n".$error->text()."\n");
};

# Not quite in the order received but at list the correct count:
print "200 OK\n" for ( $drop->file_list() );
print "450 URL is invalid\n" for ( keys(%$invalid_files) );


######################################################################
# exec squirt:

my $success = run_cmd(\@Cmd_Squirt,
                       input=>$drop->as_xml(),
                       timeout=>-1, # -1 = return immediately
                       background=>1,
                       log_file=>$Squirt_Log);

die("500 Internal Error\n".$WDIG::Run::errstr."\n") if ( !$success );

exit(keys(%$invalid_files));

__END__
