
# $Id$

use Test;

eval("require WDIG::Weblogic::Config::Default;");
if ( !@WDIG::Weblogic::Config::Default::ISA ) {
  print STDERR "\nLibXSLT not installed\n";
  plan('tests',1);
  skip(1,1);
  exit(0);
}

plan tests => 2;

import WDIG::Weblogic::Config::Default qw(setDefaults);

my $xml;

$xml = setDefaults('<Domain/>');
ok($xml =~ /LastModificationTime/);

$xml = setDefaults('<Domain Name="mydomain"><Server Name="myserver"/></Domain>');
ok($xml =~ /ListenPort=.7001/);


