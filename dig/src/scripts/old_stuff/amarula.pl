#!/usr/bin/perl
#
# amarula.pl
#
# script to extract, validate and akamize archives of content for web sites
#
# by David Kelly 2001

use strict;

# set variables

my $tardest;
my $tarsource;
my @invalid_files;
my @warning_files;
my $argument;
my $countrycode;
my $server;
my @file_list;
my $file;
my $answer;
my $mailto="matteo.moriconi\@dig.com";
my $logfile="/pool/logs/disweb/amarula.log";
my $akamaicode;
my $devdest;
my $backup="-i akabak";
my $extractdir;
my @newfile;
my $hostname;
my $temp;
my @temp;
my $ok;
my $date_start=`date`;
my $date_stop;
my $report_converted;

my @country=qw(ar at au be bo br ca ch cl cn co cz de dk ec ee es fi fr gr hk ie it jp kr lb lt lu mx nl no nz pa pe pl pr pt py ru se sg th tw uk uy ve za);

my %filecount=qw(
	cgi	0
	html	0
	image	0
	video	0
	sound	0
	java	0
	xml	0
	shock	0
	data	0);
	
my %filematch=qw!
	cgi	(cgi|pl|sh)
	html	html?
	image	(gif|jpg|png)
	video	(mov|ani|qt|mpe?g|ra|rm)
	sound	(wav|au|mp3)
	java	class
	xml	xml
	shock	swf
	data	(txt|dat|db)!;
	

# define regions - this is so that the correct akamai config file is automatically
# selected based on the country code. i have decided to keep this static for now instead of
# read from a config file as this would mean that changes in a conf type file would
# have to be copied across to other servers.

my %region=qw(
	intl	inter
	ar	asia
	at	asia
	au	asia
	be	europe
	bo	latin
	br	latin
	ca	inter
	ch	europe
	cl	inter
	cn	inter
	co	inter
	cz	europe
	de	de
	dk	europe
	ec	inter
	ee	inter
	es	europe
	fi	europe
	fr	fr
	gr	europe
	hk	asia
	ie	europe
	it	europe
	jp	asia
	kr	asia
	la	latin
	lb	inter
	lt	inter
	lu	inter
	mx	latin
	nl	europe
	no	europe
	nz	asia
	pa	inter
	pe	inter
	pl	europe
	pr	inter
	pt	europe
	py	latin
	ru	europe
	se	europe
	sg	asia
	th	asia
	tw	asia
	uk	uk
	uy	latin
	ve	latin
	za	inter
);

# check script parameters

unless (defined @ARGV)
{
	$ARGV[0]="-h";
};

for (my $count=0; $count<=$#ARGV; $count++)
{
	$argument = $ARGV[$count];
	
	if ($argument =~ "^-h")						# help
	{
		print "options:\n\n";
		print "-h		this help\n\n";
		print "-f {file}	archive file\n";
		print "-s {server}	server (dev, staging, live)\n";
		print "-m {address}	mail address for report (default: $mailto)\n";
		print "-d {directory}	specify extraction directory (default: local dir)\n";	
 		print "-n		no backups of original html files during akamaization\n";
		exit 0;
	}
	elsif ($argument eq "-f")					# specified a tarball
	{
		if ($ARGV[$count+1] !~ /.(tar|t?gz|zip)$/i)
		{
			die "specified file is not a tar or zip archive!\n\n";
		};
		$tarsource=$ARGV[$count+1];
	}
	elsif ($argument eq "-c")					# country code
	{
		$countrycode=$ARGV[$count+1];
	}	
	elsif ($argument eq "-s")					# specify the server
	{
		$server=$ARGV[$count+1];
		unless ($server eq "dev" | $server eq "staging" | $server eq "live")
		{
			die "error: server must be one of dev, staging, live!\n\n";
		};
	}
	elsif ($argument eq "-m")
	{
		$mailto=$ARGV[$count+1];
	}
	elsif ($argument eq "-d")
	{
		$tardest=$ARGV[$count+1];
	}
	elsif ($argument eq "-n")
	{
		$backup="";
	};	
};

#
# MAIN PROGRAM #
#

# first do some important checks

$hostname=`hostname`;
chomp($hostname);

#unless (`whoami` eq "disweb\n")
#{
#	die "error: this script must be run as user disweb!\n\n";
#};

unless (defined @ARGV)
{
	die "error: no options specified. please run maggie.pl -h for help\n\n";
};

if ($tarsource eq "")
{
	die "error: you must specify the archive source!\n\n";
};

unless (-e $tarsource)
{
	die "error: source archive does not exist!\n";
};

unless (defined $tardest)
{
	chomp($temp=`pwd`);
	print "no destination directory specified. use $temp ? [y/n]: ";
	chomp($answer=<STDIN>);
	if ($answer=~/^y/i)
	{
		$tardest=$temp;
	}
	else
	{
		die "error: cannot run without specified extraction directory!\n\n";
	};
};

unless (defined $server)
{
	$server="staging" if ($hostname eq "bvig-orl-di-1");
	$server="live" if ($hostname eq "bvig-orl-diadmin-1");
};	

unless (defined $countrycode)
{
	@temp=split(/\//,$tardest);
	$ok=0;
	foreach $temp (@country)
	{
		if ($temp[3] eq $temp)
		{
			$countrycode=$temp[3];
			$ok=1;
			last;
		};
	};
	unless ($ok)
	{
		print "enter correct country code: ";
		chomp ($countrycode=<STDIN>);
        };
};

unless (defined $region{$countrycode})
{
	print "enter correct region: ";
	chomp ($region{$countrycode}=<STDIN>);
};

unless (-d $tardest)
{
	mkdir($tardest,0777);
};

# turn off line buffering for a bit ..

$|=1;

# extract source

# scp tar file from dev server if we're on live

if ($server eq "live")
{
	$devdest=$tardest;
	$devdest=~s|/pool/live|/pool/dev|;
	system "scp -c blowfish 63.75.117.198:$devdest $tardest";
};

# first convert to tar if of another type

if ($tarsource =~ /.zip$/i)
{
	print "converting .zip file to .tar ... ";
	$extractdir="/tmp/".time();
	@newfile=split(/.zip$/i, $tarsource);
	mkdir($extractdir,0777);
	system "unzip -d $extractdir $tarsource > /dev/null 2>&1";
	system "chmod -R u+r+w+x $extractdir > /dev/null 2>&1";
	$tarsource="@newfile.tar";
	system "cd $extractdir ; tar -cf $tarsource *";
	system "rm -rf $extractdir";
	print "done.\n";
	$report_converted="windows zip";
};		

if ($tarsource =~ /.t?gz$/i)
{
	print "file is compressed. decompressing ... ";
	system "gunzip $tarsource";
	@newfile=split(/.zip$/i, $tarsource);
	$tarsource="@newfile.tar";
	$report_converted="gzipped"
};	

print "copying source to staging path ... ";
system "cp $tarsource $tardest > /dev/null 2>&1";
print "done.\n";

print "extracting .tar source ... ";
@file_list=`tar -xmvf $tarsource -C $tardest`;
print "done.\n";

if ($server eq "staging")
{
	$devdest=$tardest;
	$devdest=~s|/pool/staging|/pool/dev|;
	print "copying source to dev path ... ";
	system "cp $tarsource $devdest > /dev/null 2>&1";
	print "done.\n";
	system "tar -xmf $tarsource -C $devdest > /dev/null 2>&1";
};


# turn line buffering back on

$|=0;


foreach $file (@file_list)
{
	chomp ($file);
	if ($file !~ m/^[\w\/_.\-|]*$/)
	{
		push(@invalid_files,$file);
	};

	foreach $temp (keys %filematch)
	{
		$filecount{$temp}++ if $file=~/\.$filematch{$temp}$/i;
		if ($temp eq "cgi")
		{
			open(CGI,"$tardest/$file");
			while (<CGI>)
			{
				push (@warning_files,"$file may be using sendmail") if ($_=~/sendmail/);
				push (@warning_files,"$file contains the word root") if ($_=~/root/);
				push (@warning_files,"$file may run a system command") if ($_=~/system/);
			};
			chmod (0775, $tardest.$file);
			system "dos2unix $tardest/$file > /tmp/amarula-proc.tmp 2>/dev/null";
			rename ("/tmp/amarula-proc.tmp", "$tardest/$file");
			system "cp $tardest/$file $devdest/$file > /dev/null 2>&1" if ($server eq "staging");
	                chmod (0775, $tardest.$file);
		};
		if ($temp eq "invalid")
		{
			push (@invalid_files, $file);
		};
	};
};

if (defined @invalid_files)
{
	print (($#invalid_files+1)."invalid files found - remove? [y/n]: ");
	chomp($answer=<STDIN>);
	if ($answer=~/^y/i)
	{
		foreach $file(@invalid_files)
		{
			unlink $tardest.$file;
		};
	};	
};	

# add files to akamizer queue

open (QUEUE,">>/pool/flags/queue/akamai.queue") || warn "warning: could not create akamai queue!!!\n\n";

	print QUEUE "-r $backup -config /pool/$server/akamai_config/$region{$countrycode}/aka.$region{$countrycode}.staging.conf $tardest\n";

close (QUEUE);

system "touch /pool/flags/akamai.flag";

# generate a report

$date_stop=`date`;

writelog();

report();

exit 0;

#
# END OF MAIN PROGRAM
#

# generate a report

sub report()
{
	my @report;

	push (@report,"REPORT\n\n");
	push (@report, "start:         $date_start");
	push (@report, "stop:          $date_stop");
	push (@report, "\n");
	push (@report, "tar source:    $tarsource\n");
	push (@report, "original fmt:  $report_converted\n") if (defined $report_converted);
	push (@report, "$server dir:   $tardest\n");
	push (@report, "devel dir:     $devdest\n") if ($server eq "staging");
	push (@report, "country code:  $countrycode\n");
	push (@report, "\n");
	foreach $temp (sort(keys (%filematch)))
	{
		push (@report,"$temp:	$filecount{$temp}\n");
	};
	
	push (@report, "---------------\n");
	push (@report, "$#file_list	total\n");
	
	if (defined @warning_files)
	{
		push (@report,"cgi warnings:\n\n");
		foreach (@warning_files) {push (@report,"\t$_\n"); };
		push (@report,"\n");
	};
	
	if (defined @invalid_files)
	{
		push (@report,"the following files were invalid:\n\n");
		foreach (@invalid_files) {push (@report, "\t$_\n"); };
	};

	push (@report, "\n\n- end of report -\n");

	print @report;
	
	open (MAIL, "| mailx -s \"Drop execution report\" $mailto");
	
		print MAIL @report;
		
	close (MAIL);

};

# create a log file

sub writelog()
{

	my @line;
	my $line;
	my $key;
	my @temp;
	my $date;
	
	chomp($date=`/bin/date +%d-%m-%Y_%H:%M`);

	foreach $key (keys (%filematch))
	{
		push (@temp,"$key=$filecount{$key}");
	};
	push (@temp,"reject=$#invalid_files") if (defined @invalid_files);	
	push (@temp,"total=$#file_list");
	
	push (@line,$date);
	push (@line,$countrycode);
	push (@line,$tarsource);
	push (@line,$tardest);
	push (@line,join("&",@temp));
	
	open (LOG, ">> $logfile");
		print LOG join(",",@line);
		print LOG "\n";
	close (LOG);
	
};
