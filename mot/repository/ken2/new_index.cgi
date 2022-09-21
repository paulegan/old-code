#!/usr/local/bin/perl

#Author       :robert@hr.emea.mot.com 
#DevDate      :19990126
#TestedWith   :
#TestDate     :
#ReviewedBy   :
#ReviewDate   :
#ApprovedBy   :
#ProdDate     :
#History      :
#Description  :Tool for maintaining a file repository.
#Notes        :

######################################################################
# Some variables:

$cgiDir		= "/www/programs/repository";
$output		= contentsOfFile("new_main.pat");
$uploadForm	= contentsOfFile("new_upload.pat");
$buttons	= contentsOfFile("new_buttons.pat");
$header		= contentsOfFile("/www/docs/serverFiles/header");
$footer		= contentsOfFile("/www/docs/serverFiles/footer");
$leftBar	= `/www/programs/cgi-bin/leftBar/leftBar`;
$docDir		= "docs";
$iconDir	= "/images/icons";
$defaultIcon	= "${iconDir}/unknown.gif";
$allowedFileNameChars = "\-A-Za-z0-9_\/\.";
$numFilesAcross = 4;

#$| = 1;

######################################################################
# Main:

use CGI qw(:standard);
use Time::localtime;

print header;

( $currentDir = param('dir') ) =~ s|^/||;
$currentDir = $docDir if (( $currentDir eq "" )||( ! -d $currentDir ));
checkFiles($currentDir);

######
$buttons =~ s/#DIR#/$currentDir/g;
######

if ( param('mode') eq "upload" ){
  uploadFile();
}elsif ( param('mode') eq "mkdir" ){
  createDir();
}elsif ( param('mode') eq "showUploadForm" ){
  showUploadForm();
}elsif ( param('mode') eq "delete" ){
  deleteFiles();
}elsif ( param('mode') eq "move" ){
  moveFiles();
}

listDir();

######################################################################
# Subs:

sub listDir
{
 my $lsTable = ls();

 $output =~ s/#HEADER#/$header/;
 $output =~ s/#LEFTBAR#/$leftBar/;
 $output =~ s/#LS_TABLE#/$lsTable/;
 $output =~ s/#BUTTONS#/$buttons/;
 $output =~ s/#FOOTER#/$footer/;

 print $output;
}# listDir

sub uploadFile
{
 my $filename = param('theFile');
 my $dest = param('remoteLocation');
 my $tmpFilename = tmpFileName($filename);

 $filename =~ s/^.*[\\\/:]//;
 $dest = $filename if ( $dest eq "" );
 $dest = "$dest/$filename" if ( -d "$currentDir/$dest" );
 checkFiles($dest);
 system("cp $tmpFilename $currentDir/$dest");
 chmod(0664,"$currentDir/$dest"); 
 error("Could not create $currentDir/$dest.") if ( ! -e "$currentDir/$dest" );
}# uploadFile

sub showUploadForm
{
 $uploadForm =~ s/#DIR#/$currentDir/g;
 print $uploadForm;
 exit;
}# showUploadForm

sub deleteFiles
{
 local(@files,$files);

 @files = split(/,/,( $files = param('files') ));
 checkFiles(@files);
 $files =~ s/^/,/g;
 $files =~ s/,/ $docRoot/g;
 system("rm -fr $files");
}# deleteFiles

sub createDir
{
 my $newDir = param('newDir');

 checkFiles($newDir);
 umask(0002);
 mkdir("$currentDir/$newDir",0775);
 error("Could not create $currentDir/$newDir.") if ( ! -d "$currentDir/$newDir" );
}# createDir

sub moveFiles
{
 local($files,$dest);

 $dest = param('dest');
 @files = split(/,/,( $files = param('files') ));
 checkFiles(@files,$dest);
 $files =~ s/^/,/g;
 $files =~ s/,/ $docRoot/g;
 if ( -d "$docRoot$userDir/$dest" ){
   system("mv $docRoot/$files $docRoot$userDir/$dest");
 }else{
   system("mv $docRoot/$files[0] $docRoot$userDir/$dest");
 }
}# moveFiles

sub ls
{
 my (@files,$file,$icon,$link,$path,$part,$partP);
 my $count = 1;
 my $output = "";

 $output .= "<B>Current Folder:</B> ";

 opendir(DIR,"$currentDir") || die "Can't opendir $currentDir: $!";
   @files = readdir(DIR);
 closedir DIR;
 
 if ( $currentDir ne $docDir ){
   $path = "";
   $output .= "<A HREF=\"new_index.cgi\">Top</A>\n";
   $currentDir =~ /$docDir\/(.*)/;
   foreach $part ( split(/\//,$1) ){
     ($partP = $part) =~ s/[ _]/\&nbsp\;/g;
     $currentDir =~ /(.*\/$part).*/;
     $path .= " : <A HREF=\"new_index.cgi?dir=$1\">$partP</A>\n";     
   }
   $output .= $path;
 }else{
   $output .= "Top of tree\n";
 }
 
 @files = sort @files;

 $output .= "<P>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 height=51 background=\"/images/docrep_back_top.gif\">\n <img src=\"/images/docrep_blank.gif\" width=1 height=2 border=\"0\"><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<FONT face=\"MS Sans Serif\" size=\"2\" color=\"ffffff\"><b>";
 $output .= $partP;
 $output .= "</b></font><br>\n </td></tr></table>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 background=\"/images/docrep_back_middle.gif\">\n <div align=\"center\">\n <TABLE BORDER=0 WIDTH=90% background=\"/images/docrep_blank.gif\" CELLSPACING=0 CELLPADDING=10>\n <TR VALIGN=\"Top\">\n";

 foreach $file ( @files )
 {
   if ((-r "$currentDir/$file") && (-d "$currentDir/$file"))
   {
    next if ($file eq ".");
    next if ($file eq ".." && $currentDir eq $docDir);
    $icon = "$iconDir/dir.gif";
    $link = "new_index.cgi?dir=$currentDir/$file";
    if ($file eq "..")
     {$file = "Up One Level"};

    $icon = $defaultIcon if ( ! -r "/www/docs/$icon" );
    $file =~ s/ /\&nbsp\;/g;
    $file =~ s/_/\&nbsp\;/g;
 
    $output .= "  <TD ALIGN=CENTER WIDTH=25%>\n";
    $output .= "   <A HREF=\"$link\"><IMG SRC=\"$icon\" BORDER=0 WIDTH=32 HEIGHT=32></A><BR>\n";
    $output .= "   <font face=\"MS Sans Serif\" size=\"1\"><A HREF=\"$link\">$file</A></font> \n";
    $output .= "  </TD>\n";
    $output .= " </TR><TR>\n" if ( ( $count++ % $numFilesAcross ) == 0 );
   }
 }

 foreach $file ( @files )
 {
   if ((-r "$currentDir/$file") && !(-d "$currentDir/$file") )
   {
    $file =~ /(\.[^\.]*)$/;
    $type = ${1};
    $type =~ s/\.//;
    $icon = "$iconDir/$type.gif";
    $link = "$currentDir/$file";

    $icon = $defaultIcon if ( ! -r "/www/docs/$icon" );
    $file =~ s/ /\&nbsp\;/g;
    $file =~ s/_/\&nbsp\;/g;
  
    $output .= "  <TD ALIGN=CENTER WIDTH=25%>\n";
    $output .= "   <A HREF=\"$link\"><IMG SRC=\"$icon\" BORDER=0 WIDTH=32 HEIGHT=32></A><BR>\n";
    $output .= "   <font face=\"MS Sans Serif\" size=\"1\"><A HREF=\"$link\">$file</A></font> \n";
    $output .= "  </TD>\n";
    $output .= " </TR><TR>\n" if ( ( $count++ % $numFilesAcross ) == 0 );
  }
 }

 while ( ( $count++ % $numFilesAcross ) != 1 ){
   $output .= "<TD WIDTH=25%>&nbsp;</TD>";
 }

 $output .= " </TR>\n";
 $output .= "</TABLE>\n </div>\n </TD></TR></TABLE>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 height=35 background=\"/images/docrep_back_bottom.gif\">&nbsp;</td></tr></table>\n";
}# ls


sub checkFiles
{
 foreach my $file ( @_ ){
   error("Null file name: &quot;$file&quot; ") if ( $file eq "" );
   error("Invalid file name: &quot;$file&quot; ") if ( $file =~ /[^$allowedFileNameChars]/ );
   error("Invalid file name: &quot;$file&quot; ") if ( $file =~ /\.\./ );
 }
}# checkFiles

sub error
{
 $mainHeader =~ s/(<BODY[^>]*)>/$1 onLoad="alert('$_[0]');" >/;
 listDir();
 exit;
}# error

sub contentsOfFile
{
 open (FILE,"$_[0]") || die "Can't Open $_[0]: $!\n";
  @fileContents = <FILE>;
 close (FILE);
 join($",@fileContents);
}# contentsOfFile
