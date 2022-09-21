 
# $Id$
 
use ExtUtils::MakeMaker;
 
WriteMakefile(
 
    'NAME'              => 'WDIG::Content::User',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'VERSION_FROM'      => 'User.pm',
    'ABSTRACT_FROM'     => 'User.pm',
    'PREREQ_PM'         => {
                             Mail::Sendmail=>0.79,
                             Unix::PasswdFile=>0.06,
                             XML::Simple=>2.05,
                             WDIG::Content::URL_Map=>1.0,
                            },
        
);
