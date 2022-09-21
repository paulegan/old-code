
# $Id$

use Test;
use WDIG::Content::Drop;
use WDIG::Content::URL_Map;

BEGIN { plan tests => 15 };

ok(1);

my $drop_xml = <<'END';
<drop source="file:///tmp" dest="http://pingu/test">
  <file>/test.html</file>
  <file>/images/test.gif</file>
  <file>test</file>
</drop>
END

# Set the URL map file location.  This works only because 
# the URL_Map object is a singleton.
my $url_map = new WDIG::Content::URL_Map('t/url_map.xml');

my $drop = undef;

my $src = 'file:///tmp';
my $src2 = 'file:///test';
my $dst = 'http://pingu/';
my @file_list = ( 'file1', 'file2' );

$drop = new WDIG::Content::Drop($src, $dst, @file_list);
ok($drop);
ok($drop->source() eq $src);
ok($drop->dest() eq $dst);
ok($drop->source($src2) eq $src2);

ok(($drop->file_list())[0] eq $file_list[0]);
# ok($drop->file_list()->[0] eq $file_list[0]);

my @test_file_list = $drop->file_list([]);
ok(@test_file_list == 0);

@file_list = $drop->file_list(@file_list);
ok(@file_list == 2);

$drop = new WDIG::Content::Drop($drop_xml);
ok($drop);
ok($drop->as_xml() =~ /^<drop.*>$/m);

my $drop2 = $drop->new();
ok($drop2->as_xml() eq "$drop");
ok($drop2->source($src2));
ok($drop2->as_xml() ne "$drop");

{
 my $invalid = undef;
 local $SIG{__DIE__} = sub { $invalid = 1 if ( $_[0] =~ /invalid/ ) };
 eval { $drop2->source('file:///te$t') };
 ok($invalid);
}

{
 my $invalid = undef;
 local $SIG{__WARN__} = sub { $invalid = 1 if ( $_[0] =~ /invalid/ ) };
 $drop2->file_list('te$t');
 ok($invalid);
}
