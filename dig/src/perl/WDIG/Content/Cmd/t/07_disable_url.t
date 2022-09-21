
# $Id$

use Test;
use Log::Log4perl qw(:nowarn);
use WDIG::Content::Cmd;
use WDIG::Content::User;

BEGIN { plan tests => 3 };

ok(1);

my $user = new WDIG::Content::User('test42','','','http://disney.ie/test');

my $cmd;

ok($cmd = new WDIG::Content::Cmd('disable_url',user_db_file=>'t/passwd'));
ok($cmd->input($user));
#ok($cmd->exec());
#ok($cmd->output() =~ /^<user.*>$/s);
