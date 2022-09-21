#!/usr/bin/perl

use HTTP::Proxy qw(:log);

my $proxy = HTTP::Proxy->new(port => 8080);
$proxy->logmask(ALL);
print $proxy->max_clients();
$proxy->start;
