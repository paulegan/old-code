
# $Id$

use strict;
use warnings;
use Test::More;
use WDIG::Weblogic::Cmd;

unless ( $ENV{BEA_HOME} and grep(/weblogic.jar/,$ENV{CLASSPATH}) and `java weblogic.Admin` ) {
  plan skip_all => "- Weblogic not found";
}

diag("This is a real test.  A Weblogic server will be started, tested and stopped.\n");

plan tests => 5;
ok(1);

my $server = "tserver";
my $domain = "tdomain";
my $host = "localhost";
my $port = 7001;
my $passwd = "tpasswd";
my $wl_log = "logs/weblogic.log";
my $stdout_log = "logs/stdout.log";

$ENV{WL_HOME} = 't';

mkdir('t/logs');

ok(start_server($server,$domain,$passwd,log_file=>$stdout_log));

ok(ping_server($host,$port,$passwd));

ok(run_admin_cmd("VERSION",$host,$port,$passwd,success_pattern=>'WebLogic Server 6'));

ok(stop_server($host,$port,$passwd,check_log_file=>$wl_log));
