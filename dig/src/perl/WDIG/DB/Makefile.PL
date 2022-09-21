
# $Id$

use ExtUtils::MakeMaker;
 
WriteMakefile(
 
    'NAME'              => 'WDIG::DB',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'ABSTRACT_FROM'     => 'DB.pm',
    'VERSION_FROM'      => 'DB.pm',
    'PREREQ_PM'         => {
                             Error=>0.15,
                             Log::Log4perl=>0.31,
                           },
 
);
