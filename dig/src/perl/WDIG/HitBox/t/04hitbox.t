
# $Id$

use strict;
use warnings;
use Test::More;

use WDIG::HitBox::Param_Map;

plan tests => 20;

`rm t/ref.db` if ( -e 't/ref.db' );
`cd t; tar -xf docs.tar`;

ok( !-e 't/ref.db' and -d 't/docs' );

my $output;

$output = `./hitbox -c t/config.xml t/docs/store/footer.html t/docs/store/index.html`;

ok($output =~ m|store/footer.html: /store|);
ok($output =~ m|store/index.html: skipped; frameset|);
ok($output =~ m|store/footer.html: removed tag; has parent|);


$output = `./hitbox -c t/config.xml -n t/docs`;

ok($output =~ m|docs/index.html: /;/IE;/EMEA/IE|);

ok($output =~ m|docs/insert_point1.shtml: No insert point|);
ok($output =~ m|docs/insert_point2.shtml: /;/IE;/EMEA/IE|);

ok($output =~ m|bak/index.html: skipped; matched filter|);
ok($output =~ m|films/index.html: skipped; contains refresh|);

ok($output =~ m|store/index.html: skipped; frameset|);
ok($output =~ m|store/header.html: skipped; has parent|);
ok($output =~ m|store/footer.html: skipped; has parent|);
ok($output =~ m|store/main.html: /store;|);

ok($output =~ m|lumpy/index.html: /store;/LUMPY/store|);
ok($output =~ m|lumpy/test.html: skipped; has parent|);
ok($output =~ m|lumpy/test2.html: skipped; has parent|);

ok($output =~ m|films/index.jsp: /BVI|);
ok($output =~ m|films/inc1.jsp: skipped; has parent|);
ok($output =~ m|films/inc2.jsp: skipped; has parent|);
# ok($output =~ m|films/inc3.jsp: skipped; has parent|);


$output = `./hitbox -c t/config.xml -n t/docs/films/inc1.jsp`;

ok($output =~ m|films/inc1.jsp: skipped; has parent|);

1;
__END__
