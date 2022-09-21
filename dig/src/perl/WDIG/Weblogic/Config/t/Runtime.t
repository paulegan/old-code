
# $Id$

use Test;
use WDIG::Weblogic::Config::Runtime;

my $config = new WDIG::Weblogic::Config::Runtime('petstore', 
                                        'petstoreServer',
                                        wl_home=>'t',
				        use_defaults=>0,
				        cache_file=>0,
                                        passwd=>'weblogic');

print STDERR "\nRuntime needs a running Weblogic server to test\n";

plan tests => 1;
skip(1,1);

# ok($config->getElementProperty('JDBCDataSource','EstoreDB','PoolName'), 'petstorePool');

