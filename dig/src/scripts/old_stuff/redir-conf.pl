#!/usr/bin/perl
#
# update-conf.pl
#
# by David Kelly 2001
#
# this script is used to update Netscape Suitespot obj.conf files by
# adding in new redirects and generating individual config files for
# each server based on a generic config file

# set some defaults

@new_redirect=();
$conf_dir="/pool/conf/live/https-live-redirect/config/";			# warning: trailing / is imperative!!
$main="obj.conf";
$status="added-redirects";
$replaceall=0;
$restart_servers=0;
$restart_command="/opt/customer/local/bin/www-restart redirect";
$add_www=1;

# check parameters sent to the program

for ($count=0; $count<=$#ARGV; $count++)
{
	$argument = $ARGV[$count];
	
	if ($argument =~ "^-h")						# help
	{
		print "Options:\n\n";
		print "-h		this help\n";
		print "-a [orig] [dest]	add an individual redirect\n";
		print "-f [filename]	read a text file of space delimited redirects\n";
		print "-i		read whitespace delimited redirects from console\n";
		print "-r		restart servers when complete\n";
		print "-y		assume yes to redirect replacement questions\n";
		print "-w		don't add extra www.{domain} redirector\n";
		
		exit 0;
	}
	elsif ($argument eq "-f")					# specified read file with -f
	{
		open(INFILE,"<$ARGV[$count+1]") || die "couldn't open the input file $ARGV[$count+1]!\n\n";
		while (<INFILE>)
		{
			push (@new_redirect,$_);
		};
		close(INFILE) || die "couldn't close the input file $ARGV[$count+1]\n\n";
	}
	elsif ($argument eq "-i")					# input from console
	{
		print "type or paste whitespace delimited text now\n";
		while (<STDIN>)
		{
			push (@new_redirect,$_);
		};
		
	}	
	elsif ($argument eq "-a")					# specify add a redirect
	{
		unless ($ARGV[$count+1] eq "" | $ARGV[$count+2] eq "")
		{
			push(@new_redirect, "$ARGV[$count+1] $ARGV[$count+2]");
		}
		else
		{
			die "error: -a parameter with no redirects specified!\n";
		};	
	}
	elsif ($argument eq "-w")
	{
		$add_www=0;
	}
	elsif ($argument eq "-y")
	{
		$replaceall=1;
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

validate_redirects ();

if (defined @new_redirect)						# create an updated obj.conf.main if required
{
	generate_main ();
};


if ($restart_servers)
{
	system $restart_command;
};

#
# END OF MAIN PROGRAM
#

#
# VALIDATE REDIRECTS
#

sub validate_redirects
{

my @tmp_redirect;
my $www_needed;
my @temp;
my $domain;
my $search;
my %redirect;

	foreach (@new_redirect)
	{
		@temp=split;
		$redirect{$temp[0]}=$temp[1];
		push (@tmp_redirect, $_);
	};

	foreach $domain (keys %redirect)
	{
		if ($domain !~ /^www\./)
		{
			$www_needed="www.$domain $redirect{$domain}\n";
			
			foreach $search (keys %redirect)
			{
				if ($search eq "www.$domain")
				{
					$www_needed="";
				};
			};
			if ($www_needed)
			{
				push (@tmp_redirect,$www_needed)	
			};
		};
	};

@new_redirect=sort(@tmp_redirect);

};

#
# generate new redirect bits
#

sub print_redirects							
{

my $count;
my @line;
	
	foreach (@new_redirect)
	{
		@line=split;
		
		if ($doredirect[$count]==1)
		{
			print MAINOUT "<Client urlhost=\"$line[0]\">\n";
			print MAINOUT "NameTrans fn=\"redirect\" from=\"\" url-prefix=\"http://redirect.disneyinternational.com/cgi-bin/click.pl?code=REDIR.$line[0]&url=http://$line[1]\"\n";
			print MAINOUT "</Client>\n";
		};
		
		$count ++;
	};	
};

#
# GENERATE THE MAIN OBJ.CONF FILE
#

sub generate_main
{

my @line;
my $line;
my @new_file;
my @redirect_site;
my $count;

	
	open(MAININ,$conf_dir . $main) || die "couldn't open the main conf file $main!\n\n";
	my @main_file=(<MAININ>);
	close MAININ;

	# check if a redirect already exists

	foreach (@new_redirect)
	{
		($tmp)=split;
		if ($tmp) { push(@redirect_site, $tmp) };
	};
	
		
	foreach (@new_redirect)
	{
		@line=split;
		$doredirect[$count]=1;
		for ($mainline=0; $mainline<$#main_file; $mainline++)
		{
			$line=$main_file[$mainline];
			if (($line=~ /^<Client/) and ($line=~/.*\"$line[0]\">/))
			{
				$doredirect[$count]=0;
				if ($replaceall)
				{
					$main_file[$mainline+1]="NameTrans fn=\"redirect\" from=\"\" url-prefix=\"http://redirect.disneyinternational.com/cgi-bin/click.pl?code=REDIR.$line[0]&url=http://$line[1]\"\n";	
				}
				else
				{
					print "found an existing redirect for $line[0] - replace old? (yes/no/all) : ";
					$choice=<STDIN>;

					if ($choice=~/^y/)
					{
						$main_file[$mainline+1]="NameTrans fn=\"redirect\" from=\"\" url-prefix=\"http://redirect.disneyinternational.com/cgi-bin/click.pl?code=REDIR.$line[0]&url=http://$line[1]\"\n";	
					}
					elsif ($choice=~/^n/)
					{
						$status="failed-addition";
					}
					elsif ($choice=~/^a/)
					{
						$main_file[$mainline+1]="NameTrans fn=\"redirect\" from=\"\" url-prefix=\"http://redirect.disneyinternational.com/cgi-bin/click.pl?code=REDIR.$line[0]&url=http://$line[1]\"\n";	
						$status="replaced-entries";
						$replaceall=1;			
					};				
				};
			};
		};

		$count++;
	};

					
	# write the new conf file
	
	open(MAINOUT,"> $conf_dir" . "obj.conf.main.tmp") || die "couldn't write temp output file!\n\n";

	foreach (@main_file)
	{
		print MAINOUT $_;
		if (/# Redirects #/)
		{
			print_redirects ();
		};
	};
			
	close(MAINOUT) || die "warning: error writing to temp output file!\n\n";

	rename "$conf_dir"."obj.conf.main.tmp", "$conf_dir$main";

	system "ci -m$status -q -l $conf_dir$main";

};


exit 0;

