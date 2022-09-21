<?php
 
class Admin
{
  var $table = "hr_admins_tb";
  
  function getName($admin,$country)
  {
    if (ereg("^[0-9]{8}$",$admin))
    {
      $db = new DB_tcts;
      $db->query("select name from $this->table where cid='$admin' and country='$country'");
      if ($db->next_record())
         return $db->f("name");
      else return "";
    }
    else return "";
  } // function getName
  
  function isValidAdmin($admin,$country)
  {
    return($this->getName($admin,$country) != "");
  } // function isValidAdmin
  
  function isAdmin($admin)
  {
    if (ereg("^[0-9]{8}$",$admin))
    {
      $db = new DB_tcts;
      $db->query("select name from $this->table where cid='$admin'");
      return ($db->next_record());
    }
    else return false;
  } // function isAdmin
  
  function getAdminList($country)
  {
    $db = new DB_tcts;
    $db->query("select * from $this->table where country='$country' ORDER BY cid");
    $counter = 0;
    $codes;
    
    while ($db->next_record())
    {
      $codes[$db->f("cid")] = $db->f("name");
      $counter++; 
    }
    
    if ($counter == 0)
       hris_printError("No entries in the $this->table table for country $country");
       
    return $codes;
  } // function getAdminList
  
  
  function getCenterList($adminCID)
  {
    $db = new DB_tcts;
    $db->query("select cc.code as code,cc.name as name from $this->table,center_codes cc where cid = '$adminCID' and country = cc.code");
    $counter = 0;
    $codes;
    
    while ($db->next_record())
    {
      $codes[$db->f("code")] = $db->f("name");
      $counter++; 
    }
    
    if ($counter == 0)
       hris_printError("No entries in the $this->table table for admin $AdminCID");
       
    return $codes;
  } // function getAdminList
} // class Admin
?>
