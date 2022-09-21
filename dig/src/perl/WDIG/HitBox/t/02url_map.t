
# $Id$

use strict;
use warnings;
use Test::More;
use File::Spec::Functions qw(rel2abs);

use WDIG::HitBox::URL_Map;

plan tests => 15;

ok(1);

my $map = new WDIG::HitBox::URL_Map('t/url_map.xml', doc_root_base=>'t');

my $param;

$param = $map->get_params(rel2abs('t/test/index.html'));
ok(!$param);

$param = $map->get_params(rel2abs('t/docs/index.html'));
ok($param->{path} =~ m|t/docs/$|);
ok($param->{filename} eq 'index.html');
ok($param->{country} eq 'IE');

$param = $map->get_params(rel2abs('t/docs/films/index.html'));
ok($param->{country} eq 'IE');
ok($param->{unit} eq 'BVI');
ok($param->{brand} eq 'DIS');

my %params = $map->get_params(rel2abs('t/docs/store/index.html'));
ok($params{unit} eq 'NONE');
ok($params{path_less_pattern} eq 'store/');

$param = $map->get_params(rel2abs('t/pingu/index.html'));
ok($param->{country} eq 'INTL');
ok($param->{doc_root} =~ m|t/pingu$|);

$param = $map->get_params(rel2abs('t/docs/films/lumpy/index.html'));
ok($param->{brand} eq 'LUMPY');
ok($param->{path} =~ m|t/docs/films/lumpy/$|);
ok($param->{path_less_pattern} eq 'films/');


1;
__END__
