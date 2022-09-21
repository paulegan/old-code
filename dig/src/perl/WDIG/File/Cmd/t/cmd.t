
# $Id$

use Test;
use WDIG::File::Cmd;

BEGIN { plan tests => 5 };

ok(1);

umask(0000);

ok(mkdir_p("t/test1/test2"));

ok(cp_p("/etc/passwd","t/test1/test2/passwd"));

ok(cp_p("/etc/passwd","t/test1/test3/passwd"));

ok(rm_fr("t/test1"));
