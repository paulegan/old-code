
# $Id$

use Test;
use WDIG::Content::Cmd;
use Log::Log4perl qw(:nowarn);

BEGIN { plan tests => 8 };

ok(1);

my $cmd;

ok($cmd = new WDIG::Content::Cmd('null'));
ok($cmd->exec(""));
ok($cmd->output() eq "");

ok($cmd = new WDIG::Content::Cmd::null());
ok($cmd->input("") eq "");
ok($cmd->exec());
ok($cmd->output() eq "");
