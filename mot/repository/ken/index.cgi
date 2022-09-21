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
#Description  :Tool for maintaining a file repository.
#Notes        :Forcing IE3 to open the upload window in a new window results in malformed multi-part post. This is an IE3 bug.


######################################################################
# Some variables:

$cgiDir		= "/www/docs/renewal/documents/";
$serverDocRoot	= "/www/docs";
$patternFile	= "nonupload.pat";
$header		= contentsOfFile("${serverDocRoot}/serverFiles/header");
$footer		= contentsOfFile("${serverDocRoot}/serverFiles/footer");
$leftBar	= `/www/programs/cgi-bin/leftBar/leftBar`;
$docDir		= "docs";
$iconDir	= "/images/icons";
$defaultIcon	= "${iconDir}/unknown.gif";
$filesPerRow	= 3;
$allowedFileNameChars = "\-A-Za-z0-9_\/\.";
$currentDir	= $docDir;
$repositoryName	= "HR EMEA Document Repository";

$| = 1;

######################################################################
# Main:

require '/www/programs/lib/ldap.pl';

$SIG{__DIE__} = sub { error($_[0]); };

print "Content-Type: text/html\n\n";

use CGI qw(:cgi tmpFileName escape);
use Time::localtime;

( $currentDir = param('dir') ) =~ s|^/||;
$currentDir = $docDir if (( $currentDir eq "" )||( ! -d $currentDir ));
checkFiles($currentDir);

listDir($currentDir);


######################################################################
# Subs:

# ls(dir) - Returns a string containing the HTML for a table
# to display the contents of the specified directory.
sub ls
{
 my $dir = $_[0];
 my (@files,%file,$filename);
 my $count = 1;
 my $output = "";

 $output .= "<B>Current Folder:</B> ".locationHeader($dir)."<BR>\n";

 opendir(DIR,"$dir") || die "Can't opendir $dir: $!";
   @files =  grep { /^[^\.]/ } readdir(DIR);
 closedir DIR;
 @files = sort fileOrder map { $_ = "$dir/$_" } @files;
 unshift(@files,"$dir/..") unless ( $dir eq $docDir );

 $output .= "<P>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 height=51 background=\"/images/docrep_back_top.gif\">\n <img src=\"/images/docrep_blank.gif\" width=1 height=2 border=\"0\"><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<FONT face=\"MS Sans Serif\" size=\"2\" color=\"ffffff\"><b>";
 $output .= $dir;
 $output .= "</b></font><br>\n </td></tr></table>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 background=\"/images/docrep_back_middle.gif\">\n <div align=\"center\">\n <TABLE BORDER=0 WIDTH=90% background=\"/images/docrep_blank.gif\" CELLSPACING=0 CELLPADDING=10>\n <TR VALIGN=\"Top\">\n";

 foreach $filename ( @files )
 {
   if ( -r "$filename" )
   {
    %file = fileAttribs("$filename");
    $output .= "  <TD ALIGN=CENTER WIDTH=".100/$filesPerRow."%>\n";
    $output .= "   <A HREF=\"$file{'link'}\"><IMG SRC=\"$file{'icon'}\" BORDER=0 WIDTH=32 HEIGHT=32></A><BR>\n";
    $output .= "   <A HREF=\"$file{'link'}\"><FONT FACE=\"MS Sans Serif\" size=\"1\">$file{'name'}</FONT></A>\n";
    $output .= "  </TD>\n";
    $output .= " </TR><TR>\n" if ( ( $count++ % $filesPerRow ) == 0 );
  }
 }

 while ( ( $count++ % $filesPerRow ) != 1 ){
   $output .= "<TD WIDTH=".100/$filesPerRow."%>&nbsp;</TD>";
 }
 $output .= " </TR>\n";
 $output .= "</TABLE>\n </div>\n </TD></TR></TABLE>\n <TABLE border=0 cellpadding=0 cellspacing=0><TR><TD valign=\"top\" width=553 height=35 background=\"/images/docrep_back_bottom.gif\">&nbsp;</td></tr></table>\n";

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

 ( $attribs{'name'} = $file ) =~ s/[ _]/\&nbsp\;/g;
 $attribs{'name'} =~ s/.*\/(.*)/$1/;

 if ($attribs{'name'} =~ /\.\./){
   $file =~ /(.*)\/.*\/.*/;
   $attribs{'link'} = "index.cgi?dir=".escape($1);
   $attribs{'name'} = "Up one level";
 }

 return(%attribs);
}# fileAttribs


# locationHeader(dir) - Returns a string showing the current
# location. Includes a link to each part of the path.
sub locationHeader
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
}# locationHeader

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

# listDir(dir) - Prints out HTML page with explorer like view
# of the specified directory.
sub listDir
{
 my $lsTable = ls($_[0]);
 my $output = contentsOfFile($patternFile);

 $output =~ s/#TITLE#/$repositoryName/;
 $output =~ s/#HEADER#/$header/;
 $output =~ s/#LEFTBAR#/$leftBar/;
 $output =~ s/#REPNAME#/$repositoryName/;
 $output =~ s/#LS_TABLE#/$lsTable/;
 $output =~ s/#FOOTER#/$footer/;
 $output =~ s/#DIR#/$currentDir/g;

 print $output;
}# listDir


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
