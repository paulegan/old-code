#!/usr/local/bin/perl

#Author       :paul@hr.emea.mot.com 
#DevDate      :19990126
#TestedWith   :
#TestDate     :
#ReviewedBy   :robert@hr.emea.mot.com
#ReviewDate   :19990502
#ApprovedBy   :
#ProdDate     :
#History      :19990202,paul,Reorganised structure.
#History      :19990226,paul,Added PATH & PATH_WITH_LINKS as seperate elements.
#Description  :Tool for maintaining a file repository.
#Notes        :Forcing IE3 to open the upload window in a new window results in malformed multi-part post. This is an IE3 bug.


######################################################################
# Some variables:

$cgiDir		= "/www/programs/repository";
$serverDocRoot	= "/www/docs";
$patternFile	= "main.pat";
$header		= contentsOfFile("${serverDocRoot}/serverFiles/header");
$footer		= contentsOfFile("${serverDocRoot}/serverFiles/footer");
$leftBar	= `/www/programs/cgi-bin/leftBar/leftBar`;
$buttonsFile	= "${cgiDir}/buttons.pat";
$uploadFormFile	= "${cgiDir}/upload.pat";
$groupListFile	= "${cgiDir}/groupList.pat";
$helpFile	= "${cgiDir}/help.pat";
$mailProg	= "/usr/lib/sendmail -t";
$docDir		= "docs";
$iconDir	= "/images/icons";
$defaultIcon	= "${iconDir}/unknown.gif";
$filesPerRow	= 4;
$allowedFileNameChars = "\-A-Za-z0-9_\/\.";
$currentDir	= $docDir;

$| = 1;

######################################################################
# Main:

require '/www/programs/lib/ldap.pl';
require 'config.pl';

$SIG{__DIE__} = sub { error($_[0]); };

print "Content-Type: text/html\n\n";

use CGI qw(:cgi tmpFileName escape);
use Time::localtime;

( $currentDir = param('dir') ) =~ s|^/||;
$currentDir = $docDir if (( $currentDir eq "" )||( ! -d $currentDir ));
checkFiles($currentDir);

if ( param('mode') eq "upload" ){
  uploadFile(param('file'),$currentDir,param('newName'));
}elsif ( param('mode') eq "mkdir" ){
  createDir("$currentDir/".param('newDir'));
}elsif ( param('mode') eq "showUploadForm" ){
  showUploadForm($currentDir);
}elsif ( param('mode') eq "listGroupMembers" ){
  listGroupMembers();
}elsif ( param('mode') eq "help" ){
  help();
}

listDir($currentDir);


######################################################################
# Subs:


# uploadFile(filename,directory,<newname>) - Looks up temp location
# for the specified file and copies it to the specifed directory;
# optionally renaming it.
sub uploadFile
{
 my $filename = $_[0];
 my $tmpFilename = tmpFileName($filename);
 my $dest;

 error("No file specified!") if ( $filename eq "" );

 $filename =~ s/^.*[\\\/:]//;		# Strip out leading path.
 if ( ($_[2]) && ( $_[2] ne "") ){
   $dest = "$_[1]/$_[2]";
 }else{
   $dest = "$_[1]/$filename";
 }
 $dest .= "/$filename" if ( -d "$dest" );
 $dest =~ s/ /_/g;
 checkFiles($dest);
 system("csh -c 'cp $tmpFilename $dest >& /dev/null'");
 chmod(0664,"$dest"); 
 error("Could not create $dest.") if ( ! -e "$dest" );

 notifyGroupAbout($dest) if ( param('sendmail') eq "on" );
}# uploadFile


sub notifyGroupAbout
{
 my ($file,$dir,%groupMembers,@recipients,$uploadee);
 my $url = url();

 $_[0] =~ /(.*)\/(.*)/;
 $dir = $1; $file = $2;
 $url =~ s/upload\///;		# Quick hack not tell show users about a hidden upload interface!

 %groupMembers = listOfAttributes($groupName);
 foreach my $user (values(%groupMembers)){
  chomp($name = ${$user}{'cn'});
  chomp($mail = ${$user}{'mail'});
  push(@recipients,"$name <$mail>");
 }
 
 $uploadee = userIDTo('cn',remote_user());
 open(MAIL,"|$mailProg") || error("Can't open mail pipe: $!");
  print MAIL "To: ".join(",\n\t",@recipients)."\n";
  print MAIL "From: \"Web Repository\" <www\@hr.emea.mot.com>\n";
  print MAIL "Subject: Document added to Repository\n\n";
  print MAIL "A document, $file, has been added to the $repositoryName";
  if ( ${uploadee} ne "" ){
    print MAIL "\nby ${uploadee}.  ";
  }else{
    print MAIL ".\n";
  }
  print MAIL "You can access the document at the following directory:\n\n";
  print MAIL "\t${url}?dir=${dir}\n\n";
 close(MAIL);
}# notifyGroupAbout


# createDir(dir) - Creates the specified directory.
sub createDir
{
 checkFiles($_[0]);
 umask(0002);
 mkdir("$_[0]",0775);
 error("Could not create $_[0].") if ( ! -d "$_[0]" );
}# createDir


# ls(dir) - Returns a string containing the HTML for a table
# to display the contents of the specified directory.
sub ls
{
 my $dir = $_[0];
 my (@files,%file,$filename);
 my $count = 1;
 my $output = "";

 opendir(DIR,"$dir") || die "Can't opendir $dir: $!";
   @files =  grep { /^[^\.]/ } readdir(DIR);
 closedir DIR;
 @files = sort fileOrder map { $_ = "$dir/$_" } @files;
 unshift(@files,"$dir/..") unless ( $dir eq $docDir );

 $output .= "<TABLE WIDTH=500 BORDER=0 BGCOLOR=#FFFFFF CELLSPACING=0 CELLPADDING=8 BACKGROUND=\"/images/docrep_blank.gif\">\n <TR>\n";

 foreach $filename ( @files )
 {
   if ( -r "$filename" )
   {
    %file = fileAttribs("$filename");
    $output .= "  <TD ALIGN=CENTER VALIGN=TOP WIDTH=".100/$filesPerRow."%>\n";
    $output .= "   <A HREF=\"$file{'link'}\"><IMG SRC=\"$file{'icon'}\" BORDER=0 WIDTH=32 HEIGHT=32 VSPACE=5></A><BR>\n";
    $output .= "   <A HREF=\"$file{'link'}\"><FONT FACE=\"MS Sans Serif\" SIZE=\"1\">$file{'name'}</FONT></A><br>\n";
    $output .= "  </TD>\n";
    $output .= " </TR><TR>\n" if ( ( $count++ % $filesPerRow ) == 0 );
  }
 }

 while ( ( $count++ % $filesPerRow ) != 1 ){
   $output .= "<TD WIDTH=".100/$filesPerRow."%>&nbsp;</TD>";
 }
 $output .= " </TR>\n</TABLE>\n";

 return($output);
}# ls


# fileOrder() - Determines the sort order for the list of
# files.  In this case, alphabetical but directories first.
sub fileOrder
{
 if ( ( -d $a ) && ( -f $b ) ){
   return(-2);
 }elsif ( ( -f $a ) && ( -d $b ) ){
   return(2);
 }else{
   return(uc($a) cmp uc($b));
 }
}# fileOrder


# fileAttribs(file) - Returns an associative array with an element
# for the icon, name & link for the given file.
sub fileAttribs
{
 my $file = $_[0];
 my %attribs;

 if ( -d "$file" ){
   $attribs{'icon'} = "$iconDir/dir.gif";
   $attribs{'link'} = "index.cgi?dir=".escape($file);
 }else{
   $file =~ /\.([^\.]*)$/;
   $attribs{'icon'} = "$iconDir/${1}.gif";
   $attribs{'link'} = "$file";
 }
 $attribs{'icon'} = $defaultIcon if ( ! -r "$serverDocRoot/$attribs{'icon'}" );

 ( $attribs{'name'} = $file ) =~ s/[ _]/\ /g;
 $attribs{'name'} =~ s/.*\/(.*)/$1/;

 if ($attribs{'name'} =~ /\.\./){
   $file =~ /(.*)\/.*\/.*/;
   $attribs{'link'} = "index.cgi?dir=".escape($1);
   $attribs{'icon'} = "$iconDir/upfolder.gif";
   $attribs{'name'} = "Up One Folder";
 }

 return(%attribs);
}# fileAttribs


# pathWithLinks(dir) - Returns a string showing the current
# location. Includes a link to each part of the path.
sub pathWithLinks
{
 my $dir = $_[0];
 my ($output,$name);

 return("Top of tree\n") if ( $dir eq $docDir );

 $output = "<A HREF=\"index.cgi\">Top</A>\n";
 $dir =~ /$docDir\/(.*)/;
 foreach $part ( split(/\//,$1) ){
   ($name = $part) =~ s/[ _]/\&nbsp\;/g;
   $dir =~ /(.*\/$part).*/;
   $output .= " : <A HREF=\"index.cgi?dir=".escape($1)."\">$name</A>\n";	
 }

 return($output);
}# pathWithLinks


# checkFiles(file,...) - Check that each file doesn't break any rules.
# Call error() if one does.
sub checkFiles
{
 foreach my $file ( @_ ){
   if ( ($file eq "") || ($file =~ /[^$allowedFileNameChars]/) || ($file =~ /\.\./) ){
     error("Invalid file name: \\\'$file\\\' ",( $file eq $currentDir ));
   }
 }
}# checkFiles


# listGroupMembers() - Displays a list of the members of the
# group for this repository.
sub listGroupMembers
{
 my $groupList = contentsOfFile($groupListFile);

 my $cancelCode;

 if ( CGI::user_agent('MSIE 3') ){
   $cancelCode = "history.go(-1);";
 }else{
   $cancelCode = "top.close();";
   $header = $leftBar = $footer = "";
 }

 my %groupMembers = listOfAttributes($groupName);
 my $list = "";

 foreach my $user (values(%groupMembers)){
  chomp(my $name = ${$user}{'cn'});
  chomp(my $mail = ${$user}{'mail'});
  $list .= "<TR><TD ALIGN=LEFT><FONT FACE=\"Arial, Helvetica\" size=2>\&nbsp\;$name</font></td><TD ALIGN=LEFT><FONT FACE=\"Arial, Helvetica\" size=2>\&nbsp\;<A HREF=\"mailto:$mail\">$mail</A></font></td></tr>\n";
 }

 $groupList =~ s/#NAME#/$groupName/;
 $groupList =~ s/#LIST#/$list/;
 $groupList =~ s/#CANCEL_CODE#/$cancelCode/;
 print $groupList;
 exit;
}# listGroupMembers

# help() - Displays a help file for the repository
sub help
{
 my $help = contentsOfFile($helpFile);

 my $cancelCode;

 if ( CGI::user_agent('MSIE 3') ){
   $cancelCode = "history.go(-1);";
 }else{
   $cancelCode = "top.close();";
   $header = $leftBar = $footer = "";
 }

 $help =~ s/#CANCEL_CODE#/$cancelCode/;
 print $help;
 exit;
}# help

# listDir(dir) - Prints out HTML page with explorer like view
# of the specified directory.
sub listDir
{
 my $path = $_[0];
 my $lsTable = ls($path);
 my $pathWithLinks = pathWithLinks($path);
 my $output = contentsOfFile($patternFile);
 my $newWinCode = CGI::user_agent('MSIE 3')?"":"TARGET=\"uploadWin\" onClick=\"window.open('','uploadWin','width=500,height=300,resizable=1,scrollbars=yes');\"";

 my $buttons = contentsOfFile($buttonsFile);
 
 $buttons =~ s/#NEW_WIN_CODE#/$newWinCode/g;
 $buttons =~ s/#REP_NAME#/$repositoryName/g;

 $path =~ s/^$docDir//;
 $output =~ s/#HEADER#/$header/;
 $output =~ s/#LEFTBAR#/$leftBar/;
 $output =~ s/#PATH#/$path/g;
 $output =~ s/#REP_NAME#/$repositoryName/g;
 $output =~ s/#PATH_WITH_LINKS#/$pathWithLinks/g;
 $output =~ s/#LS_TABLE#/$lsTable/;
 $output =~ s/#BUTTONS#/$buttons/;
 $output =~ s/#FOOTER#/$footer/;
 $output =~ s/#DIR#/$currentDir/g;

 print $output;
}# listDir


# showUploadForm(dir) - Display the form for uploading a file to
# the specified directory.
sub showUploadForm
{
 my $output = contentsOfFile($uploadFormFile);
 my $cancelCode;

 if ( CGI::user_agent('MSIE 3') ){
   $cancelCode = "history.go(-1);";
 }else{
   $cancelCode = "top.close();";
   $header = $leftBar = $footer = "";
 }

 $output =~ s/#HEADER#/$header/;
 $output =~ s/#LEFTBAR#/$leftBar/;
 $output =~ s/#FOOTER#/$footer/;
 $output =~ s/#DIR#/$_[0]/g;
 $output =~ s/#CANCEL_CODE#/$cancelCode/g;
 $output =~ s/\n.*listGroupMembers[^\n]*\n/\n/ if $groupName eq "";

 print $output;
 exit;
}# showUploadForm


# error(message,gotoRoot?) - Displays the directory contents as per
# normal but also pops up a dialog box with the given message. If 
# gotoRoot is true then the root dir is displayed rather than the
# current.
sub error
{
 chomp($_[0]);
 $header =~ s/(<BODY[^>]*)>/$1 onLoad="alert('$_[0]');" >/;
 if ($_[1]){
   listDir($docDir);
 }else{
   listDir($currentDir);
 }
 exit;
}# error


# contentsOfFile(file) - Returns the contents of a file as a string.
sub contentsOfFile
{
 open (FILE,"$_[0]") || die "Can't Open $_[0]: $!\n";
  @fileContents = <FILE>;
 close (FILE);
 join($",@fileContents);
}# contentsOfFile


# sub deleteFiles
# {
#  local(@files,$files);
# 
#  @files = split(/,/,( $files = param('files') ));
#  checkFiles(@files);
#  $files =~ s/^/,/g;
#  $files =~ s/,/ $docRoot/g;
#  system("rm -fr $files");
# }# deleteFiles
# 
# sub moveFiles
# {
#  local($files,$dest);
# 
#  $dest = param('dest');
#  @files = split(/,/,( $files = param('files') ));
#  checkFiles(@files,$dest);
#  $files =~ s/^/,/g;
#  $files =~ s/,/ $docRoot/g;
#  if ( -d "$docRoot$userDir/$dest" ){
#    system("mv $docRoot/$files $docRoot$userDir/$dest");
#  }else{
#    system("mv $docRoot/$files[0] $docRoot$userDir/$dest");
#  }
# }# moveFiles
