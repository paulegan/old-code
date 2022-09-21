
# $Id$

use strict;
use warnings;
use Test::More;
use Log::Log4perl qw(:easy);
use Error qw(:try);

BEGIN { use_ok "WDIG::DB::SqlInterpreter::isql"; }

plan tests => 6;

Log::Log4perl->easy_init( { level=>$ERROR, layout=>'%p - %m%n' } );

ok(1);

my $defaults = {
     log_file => 't/isql.log',
     ssh => { 
       cmd => 'ssh',
       options => '-T -a -x -i /local/apps/.ssh/disdb -l disdb'
     }
   };
       
my $isql = new WDIG::DB::SqlInterpreter::isql(
                 defaults => $defaults,
                 server => 'sebek',
                 user => 'diuser',
                 password => 'online'
               );

try {
  $isql->exec("select", password=>'blah42');
} catch Error with {
  my $error = shift();
  ok($error->text() =~ /Login failed/);
};

try {
  $isql->exec("select count(*) from Xfiles;");
} catch Error with {
  my $error = shift();
  ok($error->text() =~ /Invalid object/);
};

ok($isql->exec("select count(*) from sysfiles;"));

try {
  $isql->exec("raiserror('a test error', 16, -1)");
} catch Error with {
  my $error = shift();
  ok($error->text() =~ /a test error/);
};

ok($isql->exec("t/test_script.sql"));
# print STDERR $isql->output();

1;
__END__
