
$CPAN::Config = {
  'build_cache' => q[0],
  'build_dir' => q[/share/src/perl/CPAN/tmp],
  'cache_metadata' => q[0],
  'cpan_home' => q[/share/src/perl/CPAN],
  'dontload_hash' => {  },
  'ftp' => q[/usr/bin/ftp],
  'ftp_proxy' => q[],
  'getcwd' => q[cwd],
  'gzip' => q[/bin/gzip],
  'http_proxy' => q[],
  'inactivity_timeout' => q[0],
  'index_expire' => q[42],
  'inhibit_startup_message' => q[1],
  'keep_source_where' => q[/share/src/perl/CPAN/local],
  'links' => q[],
  'make' => q[/usr/bin/make],
  'make_arg' => q[],
  'make_install_arg' => q[],
  'makepl_arg' => q[PREFIX=/share],
  'ncftp' => q[],
  'ncftpget' => q[],
  'no_proxy' => q[],
  'pager' => q[/usr/bin/less],
  'prerequisites_policy' => q[follow],
  'scan_cache' => q[atstart],
  'shell' => q[/usr/bin/tcsh],
  'tar' => q[/bin/tar],
  'term_is_latin' => q[1],
  'unzip' => q[/usr/bin/unzip],
  'urllist' => [q[file:///share/src/perl/CPAN/local/]],
# 'urllist' => [q[ftp://ftp.mirror.ac.uk/sites/ftp.funet.fi/pub/languages/perl/CPAN/]],
  'wait_list' => [],
  'wget' => q[/usr/bin/wget],
};
1;
__END__
