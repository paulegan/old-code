
# $Id$

use Test;
use WDIG::File::Cmd;

BEGIN { plan tests => 14 };

ok(1);

umask(0002);

######################################################################
# test script for md function

ok(!-d "t/dir1");
ok(md("t/dir1"),1);
ok(-d "t/dir1");

ok(!-d "t/dir2");
ok(md("t/dir2/dir2"),0);
ok(md("t/dir2/dir2",recurse=>1),1);
ok(-d "t/dir2/dir2");

ok(md("dir3",wd=>"t/dir1"),1);
ok(-d "t/dir1/dir3");

ok(md("t/dir4",mode=>0555),1);
skip($>==0,md("t/dir4/dir4"),0);

my @dirList = ( "t/dir5", "t/dir6" );
ok(md(\@dirList),1);
ok(-d "t/dir5" && -d "t/dir6" );

`rm -fr t/dir[0-9]`;
