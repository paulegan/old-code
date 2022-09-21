# $Id$
use ExtUtils::MakeMaker;
WriteMakefile(
    'NAME'		=> 'WDIG::File::Slurp',
    'VERSION_FROM'	=> 'Slurp.pm',
    'PREREQ_PM'		=> {},
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Slurp.pm',
       AUTHOR     => 'Paul Egan <perl@dig.com>') : ()),
);
