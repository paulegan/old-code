
# $Id$

use strict;
use warnings;
use Test;
use WDIG::Run;

BEGIN { plan tests => 12 };

print "NOTE: Best to run these tests directly with 'perl -I blib/lib -w t/Run.t'.\n";

ok(1);

my @cmd;

@cmd = ('echo','test');
ok(run_cmd(\@cmd));

@cmd = ('cmd_that_does_not_exist');
ok(!defined(run_cmd(\@cmd)));
ok($WDIG::Run::errstr =~ /(No such file|failed)/);

@cmd = ('perl','-e','$|=1; $c=3; while($c--){ sleep(1); print "$c\n"; }');
ok(run_cmd(\@cmd));

@cmd = ('perl','-e','$|=1; $c=3; while($c--){ sleep(4); print "$c\n"; }');
ok(!defined(run_cmd(\@cmd,timeout=>1)));
ok($WDIG::Run::errstr =~ /Timed out/);

@cmd = ('perl','-e','$|=1; $c=5; while($c--){ sleep(4); print "$c\n"; }');
ok(run_cmd(\@cmd,timeout=>-1,background=>1,log_file=>'t/log'));

my $output;
ok(run_cmd(['echo','test'],output=>\$output));
ok($output eq "test\n");

$output = "";
ok(run_cmd(["cat","-"],input=>"test",output=>\$output));
ok($output eq "test");
