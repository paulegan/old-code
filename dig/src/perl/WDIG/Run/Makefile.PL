
# $Id$

use ExtUtils::MakeMaker;
 
WriteMakefile(
 
    'NAME'              => 'WDIG::Run',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'ABSTRACT_FROM'     => 'Run.pm',
    'VERSION_FROM'      => 'Run.pm',
    'PREREQ_PM'         => {
                             WDIG::File::Slurp=>1.0,
                             WDIG::File::Cmd=>1.0,
                           },
    clean               => { FILES=>'t/log' }
 
);
