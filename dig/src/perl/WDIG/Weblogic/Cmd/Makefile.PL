
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(

    'NAME'		=> 'WDIG::Weblogic::Cmd',
    'AUTHOR'		=> 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'VERSION_FROM'	=> 'Cmd.pm',
    'ABSTRACT_FROM'	=> 'Cmd.pm',
    'PREREQ_PM'		=> {
                             File::ReadBackwards=>0.99,
                             WDIG::File::Slurp=>1.0,
                             WDIG::Run=>1.0
                            },
    clean		=> { FILES=>'t/logs t/config/tdomain/.wlstaging t/config/tdomain/applications' }

);

