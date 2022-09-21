#!/usr/local/bin/perl -w

$workingDir	= "/www/docs/hris/private/sys";
$confFile	= "$workingDir/saw.conf";
$saDir		= "$workingDir/sa";
$patDir		= "$workingDir/pat";
$headerPat	= "$patDir/header";
$footerPat	= "$patDir/footer";
$formPat	= "$patDir/form";
$cpuPat		= "$patDir/cpu";
$diskPat	= "$patDir/disk";
$memPat		= "$patDir/mem";
$pagingPat	= "$patDir/paging";
$runQueuePat	= "$patDir/runQueue";
$siteHeader	= "/www/docs/serverFiles/header";
$siteFooter	= "/www/docs/serverFiles/footer";
$siteLeftBar	= "/www/programs/cgi-bin/leftBar/leftBar";
$sar		= "/usr/sbin/sar";


use CGI qw(:cgi escape);
require $confFile;

$SIG{__DIE__} = sub { error($_[0]); };

if ( !param('host') || !param('date') ){
  printSelectForm();
}else{
  if ( param('disk') ){
    printDiskData( param('host'), param('date'), param('disk') );
  }elsif ( param('cpu') ){
    printCpuData( param('host'), param('date') );
  }elsif ( param('mem') ){
    printMemData( param('host'), param('date') );
  }elsif ( param('pg') ){
    printPagingData( param('host'), param('date') );
  }elsif ( param('q') ){
    printQData( param('host'), param('date') );
  }else{
    printGraphPage( param('host'), param('date') );
  }
}

sub printSelectForm
{
 my $output	= cof($headerPat);
 my $hosts	= `ls $saDir`;
 my $date	= `date '+%Y%m%d'`;

 chomp($date = $date-1);
 $hosts =~ s/(\w*)\s/<OPTION>$1\n/g;

 $output .= cof($siteHeader).`$siteLeftBar`.cof($formPat).cof($siteFooter).cof($footerPat);
 $output =~ s/#HOSTS#/$hosts/g;
 $output =~ s/#DATE#/$date/g;
 $output =~ s/#URL#/url()/ge;

 print header();
 print $output; 
}

sub printGraphPage
{
 my $host	= $_[0];
 my $date	= $_[1];
 my $output	= cof($headerPat);
 my $diskTemp	= cof($diskPat);
 my $hosts	= `ls $saDir`;
 my $temp;

 die("Could not open sar file!") if ( ! -r "$saDir/$host/$date" );

 $output .= cof($siteHeader).`$siteLeftBar`.cof($formPat).cof($cpuPat).cof($runQueuePat).cof($memPat).cof($pagingPat);
 for my $fs ( sort keys %{$diskMap{$host}} ){
   $temp = $diskTemp;
   $temp =~ s/#FS#/$fs/g;
   $temp =~ s/#DISK#/escape(${$diskMap{$host}}{$fs})/ge;
   $output .= $temp;
 }
 $output .= cof($siteFooter).cof($footerPat);

 chomp($date);
 $hosts =~ s/(\w*)\s/<OPTION>$1\n/g;
 $hosts =~ s/ON>$host/ON SELECTED>$host/g;

 $output =~ s/#HOST#/$host/g;
 $output =~ s/#HOSTS#/$hosts/g;
 $output =~ s/#DATE#/$date/g;
 $output =~ s/#URL#/url()/ge;

 print header();
 print $output;
}

sub printCpuData
{
 my $host	= $_[0];
 my $date	= $_[1];
 my @data;
 my $dateString;

 ( $dateString = $date ) =~ s/(\d\d\d\d)(\d\d)(\d\d)/$3\/$2\/$1/;

 print header('text/plain');
 open(SAR,"$sar -f $saDir/$host/$date -u |") || die "Couldn't open pipe: $!\n";
 while (<SAR>){
   chomp;
   if ( /^(\d\d:\d\d:\d\d).*\d/ ){
     @data = split(/[ \t]+/);
     print "$dateString $data[0], $data[1], $data[2], $data[3], $data[4]\n";
   }
 }
 close(SAR);
}

sub printQData
{
 my $host	= $_[0];
 my $date	= $_[1];
 my @data;
 my $dateString;

 ( $dateString = $date ) =~ s/(\d\d\d\d)(\d\d)(\d\d)/$3\/$2\/$1/;

 print header('text/plain');
 open(SAR,"$sar -f $saDir/$host/$date -q |") || die "Couldn't open pipe: $!\n";
 while (<SAR>){
   chomp;
   if ( /^(\d\d:\d\d:\d\d).*\d/ ){
     @data = split(/[ \t]+/);
     print "$dateString $data[0], $data[1], $data[2]\n";
   }
 }
 close(SAR);
}

sub printMemData
{
 my $host	= $_[0];
 my $date	= $_[1];
 my @data;
 my $dateString;

 ( $dateString = $date ) =~ s/(\d\d\d\d)(\d\d)(\d\d)/$3\/$2\/$1/;

 print header('text/plain');
 open(SAR,"$sar -f $saDir/$host/$date -r |") || die "Couldn't open pipe: $!\n";
 while (<SAR>){
   chomp;
   if ( /^(\d\d:\d\d:\d\d).*\d/ ){
     @data = split(/[ \t]+/);
     print "$dateString $data[0], $data[1], ".($data[2]/2048)."\n";
   }
 }
 close(SAR);
}

sub printPagingData
{
 my $host	= $_[0];
 my $date	= $_[1];
 my @data;
 my $dateString;

 ( $dateString = $date ) =~ s/(\d\d\d\d)(\d\d)(\d\d)/$3\/$2\/$1/;

 print header('text/plain');
 open(SAR,"$sar -f $saDir/$host/$date -g |") || die "Couldn't open pipe: $!\n";
 while (<SAR>){
   chomp;
   if ( /^(\d\d:\d\d:\d\d).*\d/ ){
     @data = split(/[ \t]+/);
     print "$dateString $data[0], $data[1], $data[2], $data[3], $data[4]\n";
   }
 }
 close(SAR);
}

sub printDiskData
{
 my $host	= $_[0];
 my $date	= $_[1];
 my $disk	= $_[2];
 my $time;
 my @data;
 my $dateString;

 ( $dateString = $date ) =~ s/(\d\d\d\d)(\d\d)(\d\d)/$3\/$2\/$1/;

 print header('text/plain');
 open(SAR,"$sar -f $saDir/$host/$date -d |") || die "Couldn't open pipe: $!\n";
 while (<SAR>){
   chomp;
   if ( /^(\d\d:\d\d:\d\d).*\d.*$/ ){
     $time = "$dateString $1";
   }if ( /^ *$disk / ){
     @data = split(/[ \t]+/);
     print "$time, ".($data[5]+1).", ".($data[7]+1)."\n" if ($time ne "");
     $time = "";
   }
 }
 close(SAR);
}

sub error
{
 my $output = cof($headerPat);

 $output .= cof($siteHeader).`$siteLeftBar`;
 $output .= "<P ALIGN=CENTER><FONT COLOR=RED><B>@_</B></FONT></P>\n";
 $output .= cof($siteFooter).cof($footerPat);

 print header();
 print $output; 
 exit;
}# error

sub cof
{
 my @fileContents;
 open (FILE,"$_[0]") || die "Can't Open $_[0]: $!\n";
  @fileContents = <FILE>;
 close (FILE);
 join($",@fileContents);
}# cof
