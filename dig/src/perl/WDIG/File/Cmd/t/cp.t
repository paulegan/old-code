
# $Id$

use Test;
use WDIG::File::Cmd;

BEGIN { plan tests => 21 };

ok(1);

umask(0002);

######################################################################
# test script for the mv function

`mkdir -p t/dir1/dir2 t/dir3`;
open(F,">t/file1"); print F "file1\n"; close(F);
open(F,">t/file2"); print F "file2\n"; close(F);

ok(cp("t/file1","t/dir1"),1);
ok(-e "t/dir1/file1");

ok(cp("t/file1","t/dir1/file4"),1);
ok(-e "t/dir1/file4");

ok(cp("file1","t/dir1/dir2"),0);
ok(cp("file1","dir1/dir2",wd=>"t"),1);
ok(-e "t/dir1/dir2/file1");

ok(cp("t/file1","t/dir1",preserve_path=>1),1);
ok(-e "t/dir1/t/file1");

my @fileList = ( "file1", "file2" );
ok(cp(\@fileList,"dir3",wd=>"t"),1);
ok(-e "t/dir3/file1" && -e "t/dir3/file2");

chmod(0444,"t/file2");
ok(cp("t/file2","t/dir1",preserve_mode=>0),1);
ok(((stat("t/dir1/file2"))[2]&0777)==0664);
ok(cp("t/file2","t/dir1"),1);
ok(((stat("t/dir1/file2"))[2]&0777)==0444);
skip($>==0,cp("t/file2","t/dir1/file2"),0); # will always work for root
ok(cp("t/file2","t/dir1/file2",force=>1),1);
ok(cp("t/file2","t/file3",mode=>0644),1);
ok(((stat("t/file3"))[2]&0777)==0644);

cp("t/file2","t/file1");
open(F,"<t/file1");
my $input = <F>;
close(F);
ok($input =~ /file2/);

`rm -fr t/dir[0-9] t/file[0-9]`;
