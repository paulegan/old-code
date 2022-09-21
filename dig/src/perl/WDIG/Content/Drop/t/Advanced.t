
# $Id$

use Test;
use WDIG::Content::Drop;
use WDIG::Content::URL_Map;

BEGIN { plan tests => 11 };

ok(1);

my $drop_xml = <<'END';
<drop source="file:///tmp" dest="http://app.pingu/test">
  <file>/app/pingu/test.jsp</file>
  <file>/content/images/test.gif</file>
</drop>
END

# Set the URL map file location.  This works only because 
# the URL_Map object is a singleton.
my $url_map = new WDIG::Content::URL_Map('t/url_map.xml');

my $drop = undef;

$drop = new WDIG::Content::Drop($drop_xml);
ok($drop);

ok($drop->is_advanced_platform());

ok($drop->app_name() eq 'pingu');
ok($drop->domain_name() eq 'live');

ok($drop->app_files() == 1);

ok(($drop->content_files())[0] =~ /test.gif/);

ok($drop->requires_restart(),0);
ok($drop->file_list('RESTART'));
ok($drop->requires_restart(),1);

{
 my $invalid = undef;
 local $SIG{__WARN__} = sub { $invalid = 1 if ( $_[0] =~ /invalid/ ) };
 $drop->file_list('te$t');
 ok(!$invalid);
}
