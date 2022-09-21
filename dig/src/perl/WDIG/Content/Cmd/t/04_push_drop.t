
# $Id$

use Test;
use Log::Log4perl qw(:nowarn);
use WDIG::Content::Cmd;
use WDIG::Content::Drop;
use WDIG::Content::URL_Map;

my $url_map = new WDIG::Content::URL_Map('t/url_map.xml');


eval("require WDIG::Weblogic::Application;");
if ( !@WDIG::Weblogic::Application::ISA ) {
  print STDERR "\nWDIG::Weblogic::Application not installed\n";
  plan('tests',1);
  skip(1,1);
  exit(0);
}


plan tests => 4;

ok(1);

my $cmd;

ok($cmd = new WDIG::Content::Cmd('push_drop', url_map_file=>'t/url_map.xml'));

my $empty_drop = new WDIG::Content::Drop("file:///","http://dst/");
ok($cmd->exec($empty_drop));
ok("".$cmd->output() eq "$empty_drop");
