
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(
    'NAME'		=> 'WDIG::Content::Drop',
    'VERSION_FROM'	=> 'Drop.pm',
    'PREREQ_PM'		=> { 
                             XML::Simple=>2.05,
                             URI=>1.23,
                             WDIG::Content::URL_Map=>1.0
                           },
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Drop.pm',
       AUTHOR     => 'Paul Egan <perl@dig.com>') : ()),
);

