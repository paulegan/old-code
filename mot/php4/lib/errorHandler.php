<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class for managing error conditions.
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

 define("ERROR_PAGE_TEMPLATE","sys_error.xhtml");
 define("LOG_FACILITY",LOG_LOCAL0);
 define("DEFAULT_PRIORITY",LOG_ERR);
 
 require_once("pageTemplate.php");
 
 class ErrorHandler
 {
  var $classname = "ErrorHandler";
  var $appName = "";
  
  function ErrorHandler($app = "unknown")
  {
   global $applicationName;
   if (($app=="unknown") && ("".APPLICATION_NAME))
     $this->appName = APPLICATION_NAME;
   else
     $this->appName = $app;
  }

  function logError($msg,$priority=DEFAULT_PRIORITY)
  {
   openlog("php.$this->appName",LOG_NDELAY|LOG_PID,LOG_FACILITY);
   syslog($priority, "<$priority> $msg");
   closelog();
  }
  
  function halt($msg)
  {
   $errorPage = new PageTemplate(ERROR_PAGE_TEMPLATE);
   $errorPage->set_var("message",$msg);
   $errorPage->display();
   $this->logError($msg,LOG_CRIT);
   exit();
  }
  
 }// class ErrorHandler
  
?>
