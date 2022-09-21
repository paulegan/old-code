
# $Id$

use Test;
use WDIG::Content::Cmd::Error qw(:try);
use Log::Log4perl qw(:easy);

BEGIN { plan tests => 7 };

ok(1);

Log::Log4perl->easy_init( {
  level => $DEBUG,
  file => '>t/log',
  layout => '%c %m%n' 
} );

try {

  throw WDIG::Content::Cmd::Error("my error");

} catch Error with {

  my $error = shift();
  ok($error->text() eq "my error");

};

my $log = Log::Log4perl->get_logger("my.category");
eval { throw WDIG::Content::Cmd::Error("my other error", $log); };
my $error = $@;

ok($error and $error =~ /my other error/);
ok($error =~ /line=.(\d+)/);
ok($1+5 == __LINE__);

my @log = ();
open(L,'<t/log') or ok(0);
@log = <L>;
close(L);

ok($log[0] =~ /main my error/);
ok($log[1] =~ /my.category my other error/);

unlink('t/log');
