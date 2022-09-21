
# $Id$

use ExtUtils::MakeMaker;
 
WriteMakefile(
 
    'NAME'              => 'WDIG::HitBox',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'ABSTRACT_FROM'     => 'HitBox.pm',
    'VERSION_FROM'      => 'HitBox.pm',
    'EXE_FILES'         => [ 'hitbox' ],
    'PREREQ_PM'         => {
                             MLDBM=>2.01,
                             DB_File=>1.7,
                             XML::Simple=>2.05,
                             Log::Log4perl=>0.31,
                             WDIG::File::Slurp=>1.0,
                           },
    clean               => { FILES=>'t/docs t/ref.db t/hitbox_map.cache t/url_map.cache' }
 
);
