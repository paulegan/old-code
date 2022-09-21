<?php

/*
  This is a template for all of the codes tables
*/
class CodeTable
{
  var $table = "NONEXISTANT";
  
  function getName($code,$country)
  {
    if (ereg("^[A-Z]+$",$code))
    {
      $db = new DB_tcts;
      $db->query("select name from $this->table where code='$code' and country='$country'");
      if ($db->next_record())
          return $db->f("name");
      else return "";
    }
    else return "";
  } // function getName
  
  function isValidCode($code,$country)
  {
    return($this->getName($code,$country) != "");
  } // function isValidCode
  
  function getCodes($country)
  {
    $db = new DB_tcts;
    $db->query("select * from $this->table where country='$country' ORDER BY NAME");
    $counter = 0;
    $codes;
    
    while ($db->next_record())
    {
      $codes[$db->f("code")] = $db->f("name");
      $counter++; 
    }
    
    if ($counter == 0)
       hris_printError("No codes in the $this->table table for country $country");
       
    return $codes;
  } // function getCodes
  
} // class CodeTable


/* This is a tables for each of the codes */

class RejectionCodes extends CodeTable
{
  var $table = "Rejection_Codes_tb";
} // class RejectionCodes

class TransactionCodes extends CodeTable
{
  var $table = "transaction_codes_tb";
} // class TransactionCodes

class SendingCodes extends CodeTable
{
  var $table = "Sending_Codes_tb";
} // class CenterCodes

class CenterCodes
{
  var $table = "Center_Codes_tb";
  
  function getName($code)
  {
    if (ereg("^[A-Z]+$",$code))
    {
      $db = new DB_tcts;
      $db->query("select name from $this->table where code='$code'");
      if ($db->next_record())
          return $db->f("name");
      else return "";
    }
    else return "";
  } // function getName 

  function isValidCode($code)
  {
    return($this->getName($code) != "");
  } // function isValidCode  
  
  function getCodes()
  {
    $db = new DB_tcts;
    $db->query("select * from $this->table ORDER BY NAME");
    $counter = 0;
    $codes;
    
    while ($db->next_record())
    {
      $codes[$db->f("code")] = $db->f("name");
      $counter++; 
    }
    
    if ($counter == 0)
       hris_printError("No codes in the $this->table table");
       
    return $codes;
  } // function getCodes 
} // class CenterCodes

?>
