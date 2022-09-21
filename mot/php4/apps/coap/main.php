<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	The COAP SDS application root class.
 // 
 // Created:		2001/01/29
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.1  2001/02/01 16:30:54  eganp
 // History:		Simple systax conversion of js files from version 1
 // History:		  Yet to be even parsed.  To do -
 // History:		
 // History:		Display of correct template determined in main
 // History:		Move of code from original html files to main
 // History:		Convert db handling from cursors to PHP db.
 // History:		$form (context['form']) and $db (context['db']) need to be
 // History:		 passed around.  Originally SSJS database & request objects
 // History:		 were global.
 // History:		Client side funcs need to be stored in client js file.
 // History:		Move from prints to return of html string to be inserted in
 // History:		 to templates
 // History:
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("config.php");
			
 require_once("db.php");
 require_once("errorHandler.php");
 require_once("user.php");
 require_once("userPermissions.php");
 require_once("appPermissions.php");

 class coap
 {
  var $classname = 'coap';
  var $context = array();
  
  function coap()
  {
   global $HTTP_GET_VARS,$HTTP_POST_VARS;
   $this->context['db'] = new db(DB_USER,DB_PASSWD);
   $this->context['eh'] = new ErrorHandler(APPLICATION_NAME);
   $this->context['user'] = new User();
   $this->context['userPerms'] = new UserPermissions($this->context['user']->uid(),$this->context['db']);
   $this->context['appPerms'] = new AppPermissions($this->context['db']);
   $this->context['form'] = array_merge($HTTP_GET_VARS,$HTTP_POST_VARS);
   $this->process();
  }
  
  function process()
  {
  }
  
 }// class coap

?>
