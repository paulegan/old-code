
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(
    'NAME'		=> 'WDIG::Weblogic::Config',
    'VERSION_FROM'	=> 'Config.pm',
    'PREREQ_PM'		=> { XML::Simple=>2.05,
			     WDIG::File::Slurp=>1.0
                           },
    ($] >= 5.005 ?
      (ABSTRACT_FROM => 'Config.pm',
       AUTHOR     => 'Paul Egan <perl@dig.com>') : ()),
);
