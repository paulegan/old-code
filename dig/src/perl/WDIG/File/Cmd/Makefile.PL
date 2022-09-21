# $Id$
use ExtUtils::MakeMaker;
WriteMakefile(
    'NAME'		=> 'WDIG::File::Cmd',
    'VERSION_FROM'	=> 'Cmd.pm',
    'PREREQ_PM'		=> { File::Copy=>2.0, File::Basename=>2.6 },
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Cmd.pm',
       AUTHOR     => 'Paul Egan <perl@dig.com>') : ()),
);
