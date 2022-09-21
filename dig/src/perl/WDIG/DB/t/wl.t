
# $Id$

use strict;
use warnings;
use Test::More;
# use Log::Log4perl qw(:easy);
use Error qw(:try);
use WDIG::Weblogic::Application;

BEGIN { use_ok "WDIG::DB::SqlInterpreter"; }

plan tests => 1;

# Log::Log4perl->easy_init( { level=>$ERROR, layout=>'%p - %m%n' } );

ok(1);

my $defaults = {
     log_file => 't/isql.log',
     ssh => { 
       cmd => 'ssh',
       options => '-T -a -x -i /local/apps/.ssh/disdb -l disdb'
     }
   };

my $app = new WDIG::Weblogic::Application("pingu","staging",defaults=>"/share/lib/weblogic/test_settings.xml");

my $config = $app->jdbc_config();

die("no jdbc pool config\n") if ( !$config );

my $isql = new WDIG::DB::SqlInterpreter(
                 defaults => $defaults,
                 %{$config->{(keys(%$config))[0]}}
               );

try {
  $isql->exec("t/name_test.sql");
} catch Error with {
  my $error = shift();
  die("ERROR - ".$error->text());
};

print "SUCCESS - ".$isql->output();


1;
__END__
