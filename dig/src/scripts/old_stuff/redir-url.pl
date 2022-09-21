#!/usr/bin/perl -w
#
# script to create redirects for URLs by creating an html
# page with a refresh meta tag
#
# by David Kelly 2001

use strict;

# create variables

my $country;
my $srcdir;
my $dstdir;
my $parameter;
my $count;
my $dstfile;
my @countrycode=qw(ar at au be bo br ca ch cl cn co cz de dk ec ee es fi fr gr nk ie intl it jp kr ky lb lt lu mx nl no nz pa pe pk pl pr pt py ru se sg th tw uk uy ve za);
my $check;
my $logfile="/pool/logs/redirects/redirect.url.log";

# first check parameters

unless (defined @ARGV)
{
	$ARGV[0]="-h";
};

for ($count=0; $count <= $#ARGV; $count ++)
{
	$parameter=$ARGV[$count];
	
	if ($parameter eq "-h")
	{
		print "redir-url.pl usage:\n\n";
		print "-c {code}	country code\n";
		print "-s {source dir}	source directory for redirect page\n";
		print "-d {dest url}	destination URL for redirect page\n";
		
		exit 0;
	};
	if ($parameter eq "-c")
	{
		$country=$ARGV[$count+1];
		$check=0;
		foreach (@countrycode)
		{
			$check=1 if ($country eq $_);
		};
		unless ($check) {die "error: no such country code ! \n\n";};
	};
	if ($parameter eq "-s")
	{
		$srcdir=$ARGV[$count+1];
	};
	if ($parameter eq "-d")
	{
		$dstdir=$ARGV[$count+1];
	};
};

# MAIN PROGRAM

unless (-d "/pool/live/$country/docs/$srcdir")
{
	print "error: directory /pool/live/$country/docs/$srcdir does not exist!\n\n";
	exit 1;
};

$dstfile="/pool/live/$country/docs/$srcdir/redirectsurl.htm";

open(REDIR, ">$dstfile") || die "couldn't write to destination file $dstfile !\n\n";

	print REDIR "<html>\n";
	print REDIR "	<head><title>Disney Redirecting ..</title></head>";
	print REDIR "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"0;url=$dstdir\">\n";
	print REDIR "<body bgcolor=\"#ffffff\"></body>\n";
	print REDIR "</html>\n";
	
close(REDIR);

# write to the log file

open(LOG, ">>$logfile") || die "warning: couldn't write to log file $logfile!\n\n";

	print LOG "$country	$srcdir		$dstdir\n";

close(LOG);

exit 0;
