
# $Id$

use Test;
use WDIG::File::Slurp;

BEGIN { plan tests => 6 };

ok(1);

my $fileName = "testFile";
my $contents = "one\ntwo\nthree\nfour\n";

ok(writeFile($fileName,$contents));

ok(readFile($fileName),$contents);

my @contents = readFile("testFile");
ok($#contents+1,split("\n",$contents));

ok(!defined(writeFile("/slurp_dir_not_exist/testFile",$contents)));

ok(!defined(readFile("/slurp_dir_not_exist/testFile")));

unlink($fileName);
