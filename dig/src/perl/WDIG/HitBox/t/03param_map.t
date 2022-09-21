
# $Id$

use strict;
use warnings;
use Test::More;

use WDIG::HitBox::Param_Map;

plan tests => 4;

ok(1);

my $hitbox_map = new WDIG::HitBox::Param_Map('t/hitbox_map.xml');

my $hitbox_vars = $hitbox_map->get_vars(
                                  country => 'IE',
                                  unit => 'NONE',
                                  brand => 'DIS'
                                 );

ok($hitbox_vars);

ok($hitbox_vars->{'ACCOUNT+ID+LIST'} =~ /^DM.*EN3$/);

ok($hitbox_vars->{'CONTENT+CATEGORY'} =~ /^\/.*IE$/);


1;
__END__
