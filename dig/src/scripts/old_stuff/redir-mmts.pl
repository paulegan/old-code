#!/usr/bin/perl -w
#
# update-conf.pl
#
# by David Kelly 2001
#
# this script is used to update Netscape Suitespot obj.conf files by
# adding in new redirects and generating individual config files for
# each server based on a generic config file

use strict;

# set some defaults

my $conf_file="/pool/conf/live/https-live-mmts/config/obj.conf";
my $status="added-redirects";
my $replaceall=0;
my $restart_servers=0;
my $restart_command="/opt/customer/local/bin/www-restart mmts";

my %redirect;
my @temp;
my $count;
my $argument;
my $key;

# check parameters sent to the program

unless (defined @ARGV)
{
	$ARGV[0]="-h";
};

for ($count=0; $count<=$#ARGV; $count++)
{
	$argument = $ARGV[$count];
	
	if ($argument =~ "^-h")						# help
	{
		print "Options:\n\n";
		print "-h		this help\n";
		print "-a [dir] [dest]	add an individual redirect\n";
		print "-f [filename]	read a text file of space delimited redirects\n";
		print "-i		read whitespace delimited redirects from console\n";
		print "-r		restart servers when complete\n";
		
		exit 0;
	}
	elsif ($argument eq "-f")					# specified read file with -f
	{
		open(INFILE,"<$ARGV[$count+1]") || die "couldn't open the input file $ARGV[$count+1]!\n\n";
		while (<INFILE>)
		{
			@temp=split;
			$redirect{$temp[0]}=$temp[1];
		};
		close(INFILE) || die "couldn't close the input file $ARGV[$count+1]\n\n";
	}
	elsif ($argument eq "-i")					# input from console
	{
		print "type or paste whitespace delimited text now\n";
		while (<STDIN>)
		{
			@temp=split;
			$redirect{$temp[0]}=$temp[1];
		};
		
	}	
	elsif ($argument eq "-a")					# specify add a redirect
	{
		unless ($ARGV[$count+1] eq "" | $ARGV[$count+2] eq "")
		{
			$redirect{$ARGV[$count+1]}=$ARGV[$count+2];
		}
		else
		{
			die "error: -a parameter with no redirects specified!\n";
		};	
	}
	elsif ($argument eq "-r")
	{
		$restart_servers=1;
	};
	
};

#
# MAIN PROGRAM
#

# first check if we need to add extra redirects for www.{domain}

open (CONF, $conf_file) or die "critical: couldn't open $conf_file\n\n";
open (NEW, "> /tmp/mmts.tmp") or die "critical: couldn't create temp file!\n\n";

	while (<CONF>)
	{
		print NEW $_;
		if ($_=~ /^### forwards ###/)
		{
			foreach $key (keys(%redirect))
			{
				print NEW "NameTrans fn=\"redirect\" from=\"$key\" url-prefix=\"http://$redirect{$key}\"\n";
			};
		};
	};

close (NEW) or die "critical: couldn't close temp file!\n\n";
close (CONF) or die "critical: couldn't close config file!\n\n";

# replace the old conf file with the new and check it into RCS

rename ($conf_file, "$conf_file.old");
system "mv /tmp/mmts.tmp $conf_file";
system "ci -m$status -q -u $conf_file";
system "chown root:disweb $conf_file ; chmod ug+r+w+x,o-r-w-x $conf_file";

if ($restart_servers)
{
	system "$restart_command";
};
