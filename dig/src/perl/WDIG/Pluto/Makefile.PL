
# $Id$

use ExtUtils::MakeMaker;
 
WriteMakefile(
 
    'NAME'              => 'WDIG::Pluto',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'ABSTRACT_FROM'     => 'Pluto.pm',
    'VERSION_FROM'      => 'Pluto.pm',
    'PREREQ_PM'         => {
                             RPC::XML=>0.53,
                             Error=>0.15,
                             Log::Log4perl=>0.31,
                           },
 
);
