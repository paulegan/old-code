
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(
    'NAME'		=> 'WDIG::Content::URL_Map',
    'VERSION_FROM'	=> 'URL_Map.pm',
    'PREREQ_PM'		=> { 
                             XML::Simple=>2.05,
                             URI=>1.23 
                           },
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'URL_Map.pm',
       AUTHOR     => 'Paul Egan <perl@dig.com>') : ()),
);

