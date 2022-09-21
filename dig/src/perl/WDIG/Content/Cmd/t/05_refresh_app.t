
# $Id$

use Test;
use Log::Log4perl qw(:nowarn);
use WDIG::Content::Cmd;

eval("require WDIG::Weblogic::Application;");
if ( !@WDIG::Weblogic::Application::ISA ) {
  print STDERR "\nWDIG::Weblogic::Application not installed\n";
  plan('tests',1);
  skip(1,1);
  exit(0);
}


plan tests => 7;

ok(1);

mkdir('t/test');

my $cmd;

ok($cmd = new WDIG::Content::Cmd('refresh_app', url_map_file=>'t/url_map.xml'));

ok($cmd->exec( 'lumpy test 1' ));

ok($cmd->exec( [ 'lumpy', 'test', 1 ] ));

ok($cmd->exec( { app_name=>'lumpy', domain_name=>'test', timestamp=>1 } ));

ok($cmd->exec( '<refresh app_name="lumpy" domain_name="test" timestamp="1" />' ));

ok($cmd->output() =~ /^<drop.*>$/s);
