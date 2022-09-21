
# $Id$

use Test;
use WDIG::File::Cmd;

BEGIN { plan tests => 15 };

ok(1);

umask(0002);

######################################################################
# test script for the mv function

`mkdir -p t/dir1/dir2`;
ok(mv("t/dir1/dir2","t/dir2"),1);
ok(-d "t/dir2");

ok(mv("dir2","t/dir1/dir2"),0);
ok(mv("dir2","dir1/dir2",wd=>"t"),1);
ok(-d "t/dir1/dir2");

open(F,">t/file1"); print F "file1\n"; close(F);
ok(mv("t/file1","t/dir1"),1);
ok(-f "t/dir1/file1");

ok(mv("dir1/file1",".",wd=>"t"),1);

open(F,">t/file2"); print F "file2\n"; close(F);
my @fileList = ( "file1", "file2" );
ok(mv(\@fileList,"dir1",wd=>"t"),1);

open(F,">t/file3"); print F "file3\n"; close(F);
ok(mv(\@fileList,"../file3",wd=>"t/dir1"),0);

ok(mv("t/dir1/file1","t/file3",mode=>0444),1);
ok(((stat("t/file3"))[2]&0777)==0444);

ok(mv("t/file3","t/dir1",preserve_path=>1),1);
ok(-f "t/dir1/t/file3");

`rm -fr t/dir[0-9]`;
