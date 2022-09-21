
# $Id$

use Test;
use WDIG::Content::URL_Map;

BEGIN { plan tests => 11 };

ok(1);

my $map = new WDIG::Content::URL_Map("t/url_map.xml", cache_file=>0);

ok(defined($map));

ok($map->contains('http://disney.ie'));

ok($map->to_machine('ftp://dev.disney.ie') eq 'intlftp01');

ok($map->to_machine('http://dev.disney.ie') eq 'localhost');

ok($map->to_path_root('http://disney.ie') =~ /docs$/);

ok($map->to_path_root('http://disney.ie/test') =~ /docs$/);

ok($map->is_live_url('http://disney.ie'));

ok($map->is_advanced_platform('http://store.disney.ie'));

ok($map->to_machine_type('http://iur.disney.ie') eq 'Win32');

ok($map->to_param('http://disney.ie', 'test1') eq 'value1');

