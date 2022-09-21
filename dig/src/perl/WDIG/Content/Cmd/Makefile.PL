
# $Id$

use ExtUtils::MakeMaker;

WriteMakefile(

    'NAME'		=> 'WDIG::Content::Cmd',
    'AUTHOR'            => 'Paul Egan <paul.x.egan.-nd@dig.com>',
    'VERSION_FROM'	=> 'Cmd.pm',
    'ABSTRACT_FROM'     => 'Cmd.pm',
    'EXE_FILES'         => [ 'squirt', 'squirt_rpc_server' ],
    'PREREQ_PM'         => {
                             Error=>0.15,
                             File::Touch=>0.01,
                             File::Rsync=>0.30,
                             Log::Log4perl=>0.31,
                             XML::Simple=>2.05,
                             Mail::Sendmail=>0.79,
                             WDIG::Content::Drop=>1.0,
                             WDIG::Content::URL_Map=>1.0,
                             WDIG::Content::User=>1.0,
                             WDIG::Pluto=>1.0,
                             WDIG::DB=>1.0,
                            },
       
);
