
# $Id$

use Test;
use WDIG::Content::Cmd;
use WDIG::Content::Cmd::Error qw(:try);
use Log::Log4perl qw(:nowarn);

BEGIN { plan tests => 3 };

ok(1);

my $cmd;

try {
  $cmd = new WDIG::Content::Cmd();
} catch Error with {
  my $error = shift();
  ok($error->text() eq "No command name specified");
};


try {
  $cmd = new WDIG::Content::Cmd("invalid_cmd");
} catch Error with {
  my $error = shift();
  ok($error->text() =~ /Command not found/);
};
