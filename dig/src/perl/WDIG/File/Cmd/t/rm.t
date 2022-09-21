
# $Id$

use Test;
use WDIG::File::Cmd;

BEGIN { plan tests => 12 };

ok(1);

umask(0002);

######################################################################
# test script for rm function

`mkdir -p t/dir1/dir2`;
ok(rm("dir2",wd=>"t/dir1"),1);
ok(!-d "t/dir1/dir2");
ok(rm("t/dir1"),1);

`mkdir -p t/dir1/dir2`;
ok(rm("t/dir1"),0);
ok(rm("t/dir1",recurse=>1),1);
ok(!-d "t/dir1");

# try delete from non-writable dir
`mkdir -p t/dir1/dir2`;
open(F,">t/dir1/file1"); print F "file1\n"; close(F);
chmod(0555,"t/dir1");
if ($>) { ok(rm("t/dir1",recurse=>1),0) } else { skip(1,1) };
ok(rm("t/dir1",recurse=>1,force=>1),1);
ok(!-d "t/dir1");

`mkdir -p t/dir1`;
open(F,">t/file1"); print F "file1\n"; close(F);
my @fileList = ( "t/dir1", "t/file1" );
ok(rm(\@fileList),1);
ok(!-d "t/dir1" || !-d "t/file1" );
