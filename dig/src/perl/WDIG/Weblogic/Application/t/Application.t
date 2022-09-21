
# $Id$

use Test::More;
use Cwd;
use LWP::UserAgent;

BEGIN { use_ok "WDIG::Weblogic::Application"; }
  
unless ( $ENV{BEA_HOME} and grep(/weblogic.jar/,$ENV{CLASSPATH}) and `java weblogic.Admin` ) {
  plan skip_all => "- Weblogic not found";
}
 
diag("This is a real test.  A pair of servers will be started, tested and stopped.\n");

plan tests => 14;

ok(1);

my $BEA_HOME = $ENV{BEA_HOME};
my $appName = "my_app";
my $domainName = "test";
my $wd = cwd();
my $Defaults = <<"END";
 <config>
   <passwd>tpasswd</passwd>
   <wl_home>$wd/t/run</wl_home>
   <ssh>
     <path>ssh</path>
     <options>-T -a -x</options>
     <cmd>perl -w -I $wd/blib/lib</cmd>
   </ssh>
 </config>
END

for ( "t/run/logs", "t/run/logs/$domainName" ) { mkdir($_) };
for ( "stdout", "weblogic", "transaction", "pid" ) { mkdir("t/run/logs/$domainName/$_") };

my $app = new WDIG::Weblogic::Application($appName, $domainName, feedback=>0, defaults=>$Defaults);

ok(defined($app));

ok($app->start());

ok($app->ping());

my $browser = new LWP::UserAgent(timeout=>30);

foreach my $port ( 7001, 7002 ) {

  my $request = new HTTP::Request(GET=>"http://localhost:$port/show_vars.jsp");
  my $result  = $browser->request($request);
  ok($result->is_success());
  my $content = $result->content();

  my $server_number = $port-7000;
  ok($content =~ /ListenPort = $port/);
  ok($content =~ /server = $server_number/);
  ok($content =~ /my_app.prop = my_val/);
  
}

ok($app->stop());

ok($app->flushcache());

1;
__END__
