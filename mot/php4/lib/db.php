<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class used for database access.  Simply wraps the
 //                     phplib DB_Sql class (OCI8 version).
 // 
 // Created:		2001/01/02
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.1  2001/01/19 11:49:28  eganp
 // History:		Initial revision
 // History:
 // 
 ///////////////////////////////////////////////////////////////////////////////
 
 define("WWW_SID","hriswww");
 
 require_once("db_oci8.inc");
 require_once("errorHandler.php");

 class db extends DB_Sql
 {

  var $classname = "db";
  var $eh;
 
  function db($user, $password, $db = WWW_SID)
  {
   $this->eh = new errorHandler();
   if (!$user || !$password)
     $this->eh->halt("DB object instantiated without username or password!");
   $this->User = $user;
   $this->Password = $password;
   $this->Database = $db;
  }

  function halt($msg)
  {
   $this->eh->halt("Database error: ".$msg." [".$this->Error["message"]."]");
  }
  
  function beginTransaction()
  {
   return ($this->query("SET TRANSACTION READ WRITE"));
  }
  
  function commit()
  {
   return ($this->query("COMMIT"));
  }
  
  function rollback()
  {
   return ($this->query("ROLLBACK"));
  }
  
 }// class db

?>
