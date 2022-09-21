#!/usr/local/bin/perl -w

######################################################################
# Some variables:

$| = 1;
$workingDir	= '/www/docs/hris/private/repository';
$messagePage 	= "$workingDir/message.pat";

$count = 0;
$output 	= "";
if (!($lookat = param('PATH')))
 {$lookat="";}

######################################################################
# Main:

use CGI qw(:standard);
use File::Basename;
print header;

if (!($lookat eq ""))
 {
  $linkto = "index.cgi?PATH=$lookat\/..";
  $output = "<TD VALIGN=TOP ALIGN=CENTER WIDTH=100><A HREF=\"$linkto\"><IMG SRC=\"\/images\/icons\/dir.gif\" BORDER=0><BR>Up One Level</A></TD>\n"
 }

$fileList = `ls -m $workingDir/docsroot/$lookat`;
@files = split(/,/,$fileList);

foreach my $file (@files)
{
 $count++;
 $file =~ s/\n//g;
 $file =~ s/^ //g;
 $file =~ s/$workingDir\///;
 ($name, $path, $suffix) = fileparse ($file, '\..*');
 $type = $suffix;
 $type =~ s/\.//;
 
 $linkto = "docsroot$lookat\/$name$suffix";
 
 if ($suffix eq "")
 {
  $type = "dir";
  $linkto = "index.cgi?PATH=$lookat\/$name";
  $linkto =~ s/\//%2f/g;
 }
 elsif (fileExists("\/www\/docs\/images\/icons\/$type.gif")==0)
 {$type="unknown";}
 
 $name =~ s/_/ /g;
 if (($count%6)==0)
  {$output .= "\n</TR>\n<TR>";}
 $output .= "<TD VALIGN=TOP ALIGN=CENTER WIDTH=100><A HREF=\"$linkto\"><IMG SRC=\"\/images\/icons\/$type.gif\" BORDER=0><BR>$name</A></TD>\n";
}

printViewer($output, $lookat);

######################################################################
# Subs:

sub fileExists
{ 
 open (FILE,"$_[0]") || return(0);
 close (FILE);
 return(1);
}# contentsOfFile

sub contentsOfFile
{
 open (FILE,"$_[0]") || die "Can't Open $_[0]: $!\n";
  @fileContents = <FILE>;
 close (FILE);
 join($",@fileContents);
}# contentsOfFile

sub printViewer
{
 my $filelist = $_[0];
 my $cfolder = $_[1];
 my $output = contentsOfFile($messagePage);
 my $header = contentsOfFile("/www/docs/serverFiles/header");
 my $footer = contentsOfFile("/www/docs/serverFiles/footer");
 my $leftBar = `/www/programs/cgi-bin/leftBar/leftBar`;

 if($cfolder eq "")
  {$cfolder="Top of Tree";}
 $cfolder =~ s/_/ /g;
  
 $output =~ s/#HEADER#/$header/g;
 $output =~ s/#LEFT_BAR#/$leftBar/g;
 $output =~ s/#FOOTER#/$footer/g;
 $output =~ s/#FILELIST#/$filelist/;
 $output =~ s/#CFOLDER#/$cfolder/;

 print $output;
}# printViewer
