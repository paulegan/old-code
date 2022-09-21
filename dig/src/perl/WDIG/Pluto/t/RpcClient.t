
# $Id$

my $rpc_url;

BEGIN {
 print STDERR "\nThis test requires a running copy of pluto.\n";
 print STDERR "Please enter the rpc URL. Leave blank to skip tests.\n";
 chomp($rpc_url = <STDIN>);
}

use strict;
use warnings;

use Test::More;
use Error qw(:try);
use Log::Log4perl qw(:easy);
use WDIG::File::Slurp;
use WDIG::Pluto::RpcClient;

plan skip_all => '- no rpc URL' if ( !$rpc_url );
plan tests => 2;

Log::Log4perl->easy_init( { level=>$DEBUG, layout=>'%m%n' } );

ok($rpc_url);

print STDERR "\nPlease enter a project ID.\n";
chomp(my $project_id = <STDIN>);

my $pluto = new WDIG::Pluto::RpcClient(server=>$rpc_url);

my $success = undef;

try {

  $success = $pluto->create_user_alert(
    projectId => $project_id,
    subject => "The test script",
    message => "".readFile($0)
  );
  
} catch Error with {

   my $error = shift();
   print STDERR $error->text()."\n";

};

ok($success);

1;
__END__
