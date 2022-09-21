
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(

    'NAME'		=> 'WDIG::Weblogic::Server',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'ABSTRACT_FROM'     => 'Server.pm',
    'VERSION_FROM'	=> 'Server.pm',
    'PREREQ_PM'		=> {
                             Error=>0.15,
                             XML::Simple=>2.05,
                             LWP::UserAgent=>2.0,
                             Config::Properties=>0.47,
			     WDIG::File::Slurp=>1.0,
			     WDIG::File::Cmd=>1.0,
			     WDIG::Run=>1.0,
			     WDIG::Weblogic::Config=>1.0,
                           # WDIG::Weblogic::Application=>1.0,
			     WDIG::Weblogic::Cmd=>1.0
                           },
    clean		=> { FILES=>'t/logs t/config/tdomain/.wlstaging t/config/tdomain/applications' }

);
