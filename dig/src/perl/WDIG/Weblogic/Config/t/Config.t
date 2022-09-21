
# $Id$

use Test;
use WDIG::Weblogic::Config;

BEGIN { plan tests => 12 };

ok(1);


######################################################################

my @domains = WDIG::Weblogic::Config->getDomainList('t');

ok(-e 't/config/'.$domains[0].'/config.xml');

my $config = new WDIG::Weblogic::Config('t/config/mydomain/config.xml');
ok(defined($config->{data}->{Domain}));

my $app = $config->application('DefaultWebApp');
ok($app->{Target}[0] eq 'myserver');

my $server = $config->server($app->{Target}[0]);
ok($server->{LogFileName} =~ /weblogic.log$/);

$config = new WDIG::Weblogic::Config('petstore', 
                                     wl_home=>'t',
				     use_defaults=>0,
				     cache_file=>0);
ok(defined($config->{data}->{Domain}));

my @app_list = $config->server2app('petstoreServer');
ok(@app_list == 3);
ok(grep('tour', @app_list));

my $pool_name = $config->getElementProperty('JDBCDataSource','EstoreDB','PoolName');
ok($pool_name eq 'petstorePool');

my $dataSource = $config->getElement('JDBCDataSource','EstoreDB');
ok($dataSource->{PoolName} eq 'petstorePool');
ok(!defined($dataSource->{RowPrefetchSize}));


# Only run defaults stuff if we have LibXSLT installed.
eval("require WDIG::Weblogic::Config::Default;");
if ( !@WDIG::Weblogic::Config::Default::ISA ) {
  print STDERR "\nSkipping defaults test\n";
  skip(1,1);
  exit(0);
}

$config = new WDIG::Weblogic::Config('petstore', 
                                     wl_home=>'t',
				     use_defaults=>1,
				     cache_file=>0);
ok($config->getElement('JDBCDataSource','EstoreDB')->{RowPrefetchSize} == 48);
 
