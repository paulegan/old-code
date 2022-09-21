
# $Id$

use Test::More;
use Cwd;
use Log::Log4perl qw(:easy);

BEGIN { use_ok "WDIG::Weblogic::Server"; }

if ( !@WDIG::Weblogic::Server::ISA ) {
  plan skip_all => "- Could not load WDIG::Weblogic::Server. Probably because\
                      WDIG::Weblogic::Application is not installed.";
}
  
unless ( $ENV{BEA_HOME} and grep(/weblogic.jar/,$ENV{CLASSPATH}) and `java weblogic.Admin` ) {
  plan skip_all => "- Weblogic not found";
}
 
diag("This is a real test.  A Weblogic server will be started, tested and stopped.\n");

Log::Log4perl->easy_init( { level=>$ALL, file=>">t/logs/test.log" } );


plan tests => 4;
ok(1);

my $serverName = "tserver";
my $domainName = "tdomain";
my $wd = cwd();
my $Defaults = <<"END";
 <config>
   <passwd>tpasswd</passwd>
   <wl_home>$wd/t</wl_home>
   <ssh>
     <path>ssh</path>
     <options>-T -a -x</options>
     <cmd>perl -w -I $wd/blib/lib</cmd>
   </ssh>
 </config>
END

for ( "t/logs", "t/logs/$domainName" ) { mkdir($_) };
for ( "stdout", "weblogic", "transaction", "pid" ) { mkdir("t/logs/$domainName/$_") };

my $server = new WDIG::Weblogic::Server($serverName, $domainName, defaults=>$Defaults);

ok($server->start());

sleep(2);

ok($server->ping());

ok($server->stop());

