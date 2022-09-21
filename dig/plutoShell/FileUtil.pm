
######################################################################
#
# ID:		$Id$
#
# Author:	paul.egan@disney.com
# 		2001/11/26
#
# Last update:	$Author$
# 		$Date$
#
# Description:	Collection of utility functions for dealing with files.
#
# Log:		$Log: FileUtil.pm,v $
# Log:		Revision 1.1  2002/01/29 02:49:00  pegan
# Log:		Initial revision
# Log:
#
######################################################################

use bytes;
use strict;
use warnings;
use File::Copy;
use File::Basename;

my $Mode_dir  	= 0775;
my $Mode_exec 	= 0775;
my $Mode_file 	= 0664;

my $DoChown   	= 1;
my $UID       	= 1027;
my $GID       	= 101;

my $FileTypeBin = '/bin/file';
my $WordFile  	= '/usr/dict/words';

my $Ext_bin	= '\.(gif|jpg|swf|rm|mov)$';
my $Ext_html	= '\.s?html?$';


######################################################################
# slurpFile() - reads the contents of a file in to a string
#

sub slurpFile
{
 my $file = $_[0];
 my $contents = "";
 local $/ = undef;

 if (-e $file && open(F,"<$file")){
   $contents = <F>;
   close(F);
 }
 return ($contents);
}


######################################################################
# rm_fr() - Removes a file or directory. Equivalent to "rm -fr"
#

sub rm_fr
{
 my $file = $_[0];

 if ( -d $file ){
   chmod(0777,$file);
   if (opendir(DIR,$file)){
     my @files = grep {! /^\.\.?$/} readdir(DIR);
     closedir(DIR);
     for (@files){ /^(.+)$/; rm_fr("$file/$1"); }
     return (0) if (!rmdir($file));
   }else{
     return (0);
   }
 }else{
   return (0) if (!unlink($file));
 }

 return (1);
}


######################################################################
# mkdir_p() - creates a directory with parents like "mkdir -p"
#

sub mkdir_p
{
 my $dir = $_[0];
 my $parent = dirname($dir);

 return (0) if ( -e $dir );
 return (0) if ( ! -e $parent && !mkdir_p($parent) );
 return (0) if ( !mkdir($dir,$Mode_dir) );

 chown($UID,$GID,$dir) if ($DoChown);

 return (1);
}


######################################################################
# cp_p() - Copies a file maintaining file permissions and creating
# any necessary parent dirs.
#

sub cp_p
{
 my $source = $_[0];
 my $dest = $_[1];
 my $parent = dirname($dest);

 mkdir_p($parent) if ( ! -e $parent );

 return (0) if (!copy($source,$dest));

 my $sourceMode = (stat($source))[2];
 my $destMode = (stat($dest))[2];

 chmod($sourceMode & 0777,$dest) if ($destMode != $sourceMode);

 chown($UID,$GID,$dest) if ($DoChown);

 return (1);
}


######################################################################
# sanitizeFile() - Sets the permissions on a file as appropriate
# and converts text files to unix EOL format.
#

sub sanitizeFile
{
 my $file = $_[0];

 return (0) if ( ! -e $file );

 my $type = `$FileTypeBin $file`;

 if ( -d $file ){
   chmod($Mode_dir,$file);
 }elsif ( $type =~ /executable/ ){
   chmod($Mode_exec,$file);
 }else{
   chmod($Mode_file,$file);
 }

 if ( $file !~ /$Ext_bin/ && ( $file =~ /$Ext_html/ || $type =~ /:.*(text|script)/ ) ){   
   my $contents = slurpFile($file);
   $contents =~ s/\r\n/\n/g;
   $contents =~ s/\r\r/\n/g;
   $contents =~ s/\r/\n/g;
   if (open(F,">$file")){
     print F $contents;
     close(F);
   }
 }

 return (1);
}


######################################################################
# genPasswd() - simple password generator.  Takes 6 letters from a
# word in /usr/dict/words and appends a number between 0 & 100.
#

sub genPasswd
{
 my $passwd;
 my @words;

 if (open(WORDS,"<$WordFile")){
   @words = <WORDS>;
   close(WORDS);
 }else{
   @words = ('passwd');
 }

 do {
  $passwd = $words[rand($#words)];
 } until ($passwd =~ /(\w{6})/);
 $passwd = $1.int(rand(100));

 return ($passwd);
}

1;

