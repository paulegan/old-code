
# $Id$

use strict;
use warnings;
use Test::More;

use WDIG::HitBox;

plan tests => 6;

ok(1);

ok(canonpath('./xx///xx/.././xx/') eq 'xx/xx');

`cp t/tag.txt t/tag.tmp; chmod +w t/tag.tmp`;

ok(remove_tag('t/tag.tmp'));
ok( -z 't/tag.tmp' );
ok(!remove_tag('t/tag.tmp'));
ok(unlink('t/tag.tmp'));

1;
__END__
