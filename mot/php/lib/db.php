<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class used for database access.  Simply wraps the
 // 			phplib DB_Sql class.
 // 
 // Created:		2001/01/02
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Name$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/01/02 11:52:58  eganp
 // History:		Updated header
 // History:
 // History:		Revision 1.1.1.1  2001/01/02 11:00:44  eganp
 // History:		Initial import
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////
 
 require_once("db_oci8.inc");

 class db extends DB_Sql {

   var $classname = "db";
   var $Database = "hriswww";
 
   function db($user,$password)
   {
    $this->User = $user;
    $this->Password = $password;
   }

   function halt($msg)
   {
     printf("</td></table><b>Database error:</b> %s<br>\n", $msg);
     printf("<b>ORACLE Error</b>: %s<br>\n",$this->Error["message"]);
     die("");
   }

 }// class db

?>
