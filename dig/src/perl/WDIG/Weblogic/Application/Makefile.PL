
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(

    'NAME'		=> 'WDIG::Weblogic::Application',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'VERSION_FROM'	=> 'Application.pm',
    'ABSTRACT_FROM'     => 'Application.pm',
    'EXE_FILES'         => [ 'weblogic' ],
    'PREREQ_PM'		=> {
                             XML::Simple=>2.05,
                             Error=>0.15,
                             Config::Properties=>0.47,
			     WDIG::File::Slurp=>1.0,
                             WDIG::Weblogic::Config=>1.0,
                             WDIG::Weblogic::Server=>1.0,
                           },
    clean		=> { FILES=>'t/run/logs t/run/config/test/.wlstaging t/run/config/test/applications' }
    
);
