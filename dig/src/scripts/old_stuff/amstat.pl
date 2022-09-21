#!/usr/bin/perl -w 
#
# script to analyse amarula.log log files
#
# by David Kelly 2001

# includes

use strict;
use Time::Local;

# defaults

my $logfile="/pool/logs/disweb/amarula.log";
my $mailto="none";

# definitions

my $arg;
my @line;
my %stats_country;
my %stats_filetype;
my %stats_perday;
my @filecount;
my $temp;
my @temp;
my $count;
my $drops_total=0;
my @report;

my $date_start;
my $date_stop;
my $date_line;
my %date_start;

# first check parameters

unless (defined @ARGV)
{
	$ARGV[0]="-h";
};

for ($count=0; $count<=$#ARGV; $count++)
{
	$arg=$ARGV[$count];
	if ($arg eq "-h")
	{
		print "-h		help\n";
		print "-s		start date\n";
		print "-e		end date\n";
		print "-m {addr}	address to mail report to. use '-m none' to suppress mail\n";
		print "-w		week report (i.e. 7 days backwards from today)\n\n";
		print "date format: d-m-yyyy\n\n";
		
		exit 0;
	}
	elsif ($arg eq "-s")
	{
		@temp=split("-",$ARGV[$count+1]);
		$date_start=epoch(@temp);
	}
	elsif ($arg eq "-e")
	{
		@temp=split("-",$ARGV[$count+1]);
		$date_stop=epoch(@temp);
	}
	elsif ($arg eq "-m")
	{
		$mailto=$ARGV[$count+1];
	}
	elsif ($arg eq "-w")
	{
		@temp=split("-",`date +%d-%m-%Y`);
		$date_stop=epoch(@temp);
		$date_start=$date_stop-604800;
	};	
};


# validate a few things

unless (defined $date_start && defined $date_stop)
{
	print "error: you must specify start and end dates!\n";
	exit 0;
};

# main program

open (LOG,$logfile) || die "error: couldn't open $logfile !\n\n";

while (<LOG>)
{
	#forget year for the moment
	
	@line=split(",",$_);
	@temp=split("-",(split("_",$line[0]))[0]);
	$date_line=epoch(@temp);
	
	if ($date_line >= $date_start && $date_line <= $date_stop)
	{
		
		$stats_perday{$date_line}++;
		$stats_country{$line[1]}++;
	
		@filecount=split ("&",$line[4]);
		foreach $temp (@filecount)
		{
			@temp=split("=",$temp);
			$stats_filetype{$temp[0]}+=$temp[1];
		};
		$drops_total++;
	};
};

close (LOG);

#die "no drops in log for specified interval!\n\n" if ($drops_total==0);

# write the report

push (@report,"REPORT - amarula.log analysis\n\n");
push (@report,"start date:	".nicedate($date_start)."\n");
push (@report,"stop date:	".nicedate($date_stop)."\n");

push (@report,"\ndrops per day:\n\n");

foreach (sort(keys(%stats_perday)))
{
	push (@report,"".nicedate($_)."		$stats_perday{$_}\n");
};

push (@report,"\n\ndrops per country:\n\n");

foreach (sort(keys(%stats_country)))
{
	push (@report,sprintf("%-8s%6s\n",$_,$stats_country{$_}));
};
push (@report,"--------------\n");

push (@report,sprintf("%-8s%6s\n","total",$drops_total));

push (@report,"\n\nfile types:\n\n");

foreach (sort(keys(%stats_filetype)))
{
	unless ($_ eq "total")
	{
		push (@report,sprintf("%-8s%6s\n",$_,$stats_filetype{$_}));
	};
};

push (@report,"--------------\n");
push (@report,sprintf("%-8s%6s\n","total",$stats_filetype{total}));

print @report;

unless ($mailto="none")
{
	open (MAIL,"| mail -s 'amarula stats report' $mailto");
		print MAIL @report;
	close (MAIL);
};

exit 0;

#
# END OF MAIN PROGRAM
#

# format the date for reporting

sub nicedate()
{
	my @temp;
	@temp=split(/\s+/,localtime($_[0]));
	return (sprintf("%s %-2s %s %s",$temp[0],$temp[2],$temp[1],$temp[4]));
};

# return epoch time

sub epoch()
{
	return timegm("0","0","0",$_[0],$_[1]-1,$_[2]-1900);
};
