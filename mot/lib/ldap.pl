#!/usr/local/bin/perl -w 

# Author	: paul@hr.emea.mot.com 	
# DevDate	: 19980817
# TestedWith	:
# TestDate	: 
# ReviewedBy	: paul
# ReviewDate	: 19980820
# ProdDate	:
# ApprovedBy	:
# History	: joel,19980819,Completed function set
# History	: paul,19980903,Added listOfAttributes()
# History	: paul,19981221,Fixed bug in deleteUserAttribute()
# Description	: Set of functions used by CGIs to get user info from Enterprise Server LDAP DB.
# Notes		: Requires "ldapmodify" & "ldapsearch" from Netscape.
# Keywords	:


######################################################################
# Some variables:

$suitespotDir		= "/www/suitespot";
$ENV{'LDAP_BASEDN'}	= "";
$ENV{'LD_LIBRARY_PATH'}	= "$suitespotDir/lib";
$ldapDir		= "$suitespotDir/userdb/ldap";
$ldapmodify		= "$ldapDir/tools/ldapmodify -C $ldapDir/config/lcache.conf";
$ldapsearch		= "$ldapDir/tools/ldapsearch -C $ldapDir/config/lcache.conf";


######################################################################
# userIDTo('attribName',uid) - given a uid, returns an attrib for the
# corresponding object.  
#
sub userIDTo
{
 my %person = ldapSearch("uid=$_[1]");
 my ($dn,$attribs) = each(%person);
 chomp(my $value = ${$attribs}{$_[0]});
 return($value);
}# userDNTo


######################################################################
# userDNTo('attribName',dn) - given a dn, returns an attrib for the
# corresponding object.  
#
sub userDNTo
{
 my %person = ldapSearch("(&(dn=$_[1])(objectclass=person)");
 my ($dn,$attribs) = each(%person);
 chomp(my $value = ${$attribs}{$_[0]});
 return($value);
}# userDNTo


######################################################################
# getUserID(dn) - given a persons dn, returns their uid.
#
sub getUserID
{
  return &userDNTo("uid",$_[0]);
}


######################################################################
# getUserDN(uid) - given a persons uid, returns their dn.
#
sub getUserDN
{
  return &userIDTo("dn",$_[0]);
}


######################################################################
# getUserAttributes(uid) - given a uid , returns a list of all 
# the attributes in an associative array.
#
sub getUserAttributes(uid)
{
 my %attributes = ldapSearch("uid=$_[0]");
 my ($dn,$user) = each(%attributes);
 foreach my $key ( keys(%{$user}) ){
     chomp(${$user}{$key});
 }
 return %{$user};
}


######################################################################
# getUserAttributesFromDN(dn) - given a dn , returns a list of all the 
# attributes in an associative array.
#
sub getUserAttributesFromDN
{
 my %attributes = ldapSearch("(&($_[0])(objectclass=person))");
 my ($dn,$user) = each(%attributes);
 foreach my $key ( keys(%{$user}) ){
     chomp(${$user}{$key});
 }
 return %{$user};
}

######################################################################
# listOfGroups(<userID>) - Without an arg, returns the name of
# every groups. With a userID returns the names of all the groups 
# the user is a member of.
sub listOfGroups
{
  my @groupList;
  
  if ( defined($_[0]) ){ 
     my $userDN = getUserDN($_[0]);
     @groupList = listOfGroupsFromDN($userDN);
  }else{
     @groupList = listOfGroupsFromDN();
  }
  
  return @groupList;
}


######################################################################
# listOfGroupsFromDN(<userDN>) - Without an arg, returns the name of
# every groups. With a userDN returns the names of all the groups 
# the user is a member of.
sub listOfGroupsFromDN
{
  my @groupList;
  my %groupData;
  
  if ( defined($_[0]) ){
     %groupData = ldapSearch("(&(objectclass=groupofuniquenames)(uniquemember=$_[0]))");
  }else{
     %groupData = ldapSearch("objectclass=groupofuniquenames");
  }
  foreach my $group (values(%groupData)){
    chomp(my $groupName = ${$group}{'cn'});
    push(@groupList,$groupName);
  }
  
  return @groupList;
}

######################################################################
# listOfUserDNs(<groupCN>) - Without an arg, returns a dn for every
# person.  With a groupCN, returns dn for everyone in that group.
#
sub listOfUserDNs
{
 my @userList;

 if ( defined($_[0]) ){
   my %group = ldapSearch("(&(objectclass=groupOfUniqueNames)(cn=$_[0]))");
   my ($dn,$attribs) = each(%group);
   @userList = split(/\n/,${$attribs}{'uniquemember'});
 }else{
   %everybody = ldapSearch('objectclass=person');
   @userList = keys(%everybody);
 }

 return(@userList);
}# listOfUserDNs


######################################################################
# listOfUserIDs(<groupCN>) - Without an arg, returns a uid for every
# person.  With a groupCN, returns uid for everyone in that group.
#
sub listOfUserIDs
{
 my @userList;
 my %everybody = ldapSearch('objectclass=person');

 if ( defined($_[0]) ){
   foreach $dn ( listOfUserDNs($_[0]) ){
     if ( defined($everybody{$dn}) ){
       chomp(my $uid = ${$everybody{$dn}}{'uid'});
       push(@userList,$uid);
     }
   }
 }else{
   foreach my $person ( values(%everybody) ){
     chomp(my $uid = ${$person}{'uid'});
     push(@userList,$uid);
   }
 }

 return(@userList);
}# listOfUserIDs


######################################################################
# ldapSearch(<query>) - Opens a pipe to the ldapsearch script and
# performs the specified query.  Results are returned in a hashed
# array of hashed array references!  Top level array uses dn as the
# key. Next level uses attrib name.  Attrib values are not chomped
# and multi-values are \n seperated.  If query is not specified then
# "cn=*" is used.
#
sub ldapSearch
{
 my $hashRef = newHash();
 my $query = "cn=*";
 my %hashArray;
 my $dn;

 $query = $_[0] if defined($_[0]);
 open(PIPE,"$ldapsearch \"$query\"|") || die "Couldn't open pipe! \! ";
 while(<PIPE>){
   ($name,$value) = split(/: /);
   if ( /^$/ ){
     chomp($dn = ${$hashRef}{'dn'});
     $hashArray{$dn} = $hashRef;
     $hashRef = newHash();
   }else{
     ${$hashRef}{$name} .= $value;
   }
 }
 close(PIPE);

 if ( defined(${$hashRef}{'dn'}) ){
   chomp($dn = ${$hashRef}{'dn'});
   $hashArray{$dn} = $hashRef;
 }

 return(%hashArray);
}# ldapSearch


sub newHash { return { @_ } }


######################################################################
# addUserAttribute(attributeName,attributeValue,userID) - This 
# adds an attribute with the new value to the users record.
#
sub addUserAttribute
{
  my $userAttribute = $_[0];
  my $newValue = $_[1];
  my $userID = $_[2];
  my $userDN = &userIDTo("dn",$userID);
  
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: $userDN\n";
  print PIPE "changetype: modify\n";
  print PIPE "add: $userAttribute\n";
  print PIPE "$userAttribute: $newValue\n";
  close(PIPE);
}


######################################################################
# replaceUserAttribute(attributeName,newValue,userID) - This function 
# takes a name of the attibute to replace , the new value and the 
# userID, Note it should only be used on UNIQUE attributes.
# 
sub replaceUserAttribute
{
  my $userAttribute = $_[0];
  my $newValue = $_[1];
  my $userID = $_[2];
  my $userDN = &userIDTo("dn",$userID);
  
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: $userDN\n";
  print PIPE "changetype: modify\n";
  print PIPE "replace: $userAttribute\n";
  print PIPE "$userAttribute: $newValue\n";
  close(PIPE);
}


######################################################################
# deleteUserAttribute(attributeName,attributeValue,userID) - This 
# function deletes the attribute with the given name and value from the 
# users record.
#
sub deleteUserAttribute
{
  my $userAttribute = $_[0];
  my $currValue = $_[1];
  my $userID = $_[2];
  my $userDN = &getUserDN("dn",$userID);
  
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: $userDN\n";
  print PIPE "changetype: modify\n";
  print PIPE "delete: $userAttribute\n";
  print PIPE "$userAttribute: $currValue\n";
  close(PIPE);
}


######################################################################
# deleteGroupAttribute(attributeName,attributeValue,groupName) - This 
# function deletes the attribute with the given name and value from the 
# group.
#
sub deleteGroupAttribute
{
  my $attribute = $_[0];
  my $currValue = $_[1];
  my $groupName = $_[2];
     
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: cn=$groupName\n";
  print PIPE "changetype: modify\n";
  print PIPE "delete: $attribute\n";
  print PIPE "$attribute: $currValue\n";
  close(PIPE);
}


######################################################################
# addGroupAttribute(attributeName,attributeValue,groupName) - This 
# adds an attribute with the new value to the group.
#
sub addGroupAttribute
{
  my $attribute = $_[0];
  my $currValue = $_[1];
  my $groupName = $_[2];
  
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: cn=$groupName\n";
  print PIPE "changetype: modify\n";    
  print PIPE "add: $attribute\n";
  print PIPE "$attribute: $currValue\n";
  close(PIPE);
}

######################################################################
# deleteUserFromGroup(userID,groupName) - This function deletes a user
# from a group.
#
sub deleteUserFromGroup
{
  my $userID = $_[0];
  my $groupName = $_[1];
  my $userDN = &userIDTo("dn",$userID);
  
  &deleteGroupAttribute("uniquemember",$userDN,$groupName);
}


######################################################################
# addUserToGroup(userID,groupName) - This function deletes a user from
# a group.
#
sub addUserToGroup
{
  my $userID = $_[0];
  my $groupName = $_[1];
  my $userDN = &getUserDN($userID);
  
  &addGroupAttribute("uniquemember",$userDN,$groupName);
}


######################################################################
# createNewUser(userID,firstName,surname) - This function creates a 
# user  
#
sub createNewUser
{
  my $userID = $_[0];
  my $givenname = $_[1];
  my $surname = $_[2];

  
  open(PIPE,"|$ldapmodify > /dev/null") ||  die "Can't open pipe: $!\n";
  print PIPE "dn: uid=$userID\n";
  print PIPE "changetype: add\n";
  print PIPE "objectclass: top\n";
  print PIPE "objectclass: person\n";
  print PIPE "objectclass: organizationalPerson\n";
  print PIPE "objectclass: inetOrgPerson\n";
  print PIPE "uid: $userID\n";
  print PIPE "givenname: $givenname\n";
  print PIPE "sn: $surname\n";
  print PIPE "cn: $givenname $surname\n";
  close(PIPE);
}

######################################################################
# listOfAttributes(<groupName>) - give a group name returns a list of 
# all attributes. Otherwise returns a list of attributes for all people
# Results are returned in a hashed array of hashed array references!
# Top level array uses dn as the key. Next level uses attrib name.  
# Attrib values are not chomped and multi-values are \n seperated.
#
sub listOfAttributes 
{
  my %userList;
  my %everybody = ldapSearch('objectclass=person');

  if ( defined($_[0]) ){
    foreach $dn ( listOfUserDNs($_[0]) ){
      $userList{$dn} = $everybody{$dn} if defined($everybody{$dn});
    }
  }else{
    %userList = %everybody;
  }
  
  return %userList;
}# listOfAttributes

1;
