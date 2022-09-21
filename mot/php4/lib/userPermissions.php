<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class used to access permissions for a user.
 //			Permissions are defined by operations that can be performed
 //			on data sets.
 //
 // Created:		2001/01/08
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.1  2001/01/19 11:49:29  eganp
 // History:		Initial revision
 // History:
 // 
 ///////////////////////////////////////////////////////////////////////////////

 define('NO_ACCESS_PAGE_TEMPLATE','sys_noAccess.xhtml');
 
 require_once("errorHandler.php");
 require_once("pageTemplate.php");
 require_once("db.php");

 class UserPermissions
 {
  var $classname = 'UserPermissions';
  var $ops = array();	// Permissions by op
  var $datasets = array();	// Permissions by data
  var $ignoreNoPerms = false;
  var $eh;
  var $userID;
  var $db;

  function UserPermissions($userID,&$db,$ignoreNoPerms=false)
  {
   global $PHP_SELF;
   
   $this->eh = new errorHandler();
   
   if (!$userID)
     $this->eh->halt("UserPermissions object instantiated without user ID!");
   $this->userID = $userID;
   
   if (!$db || $db->classname!='db')
     $this->eh->halt("DB object not given to UserPermissions!");
   $this->db = &$db;  
   
   $this->ignoreNoPerms = $ignoreNoPerms;
   if (!$ignoreNoPerms && !$this->getPerms()){
     $this->eh->logError("Access denied for ".$this->userID." to ".$PHP_SELF);
     $page = new PageTemplate(NO_ACCESS_PAGE_TEMPLATE);
     $page->set_var('user',$this->userID);
     $page->display();
     exit();
   }
  }
  
  function getPerms()
  {
   global $PHP_SELF;

   if ($this->ops) return ($this->ops);
   
   $this->db->query("SELECT operation,dataset FROM perm WHERE user_id = '".strtoupper($this->userID)."' ORDER BY operation");

   while ($this->db->next_record()){
     $this->ops[$this->db->f('operation')][] = $this->db->f('dataset');
     $this->datasets[$this->db->f('dataset')][] = $this->db->f('operation');
   }
   
   return ($this->ops);
  }
  
  function canDoOp($op,$dataset=0)
  {
   $this->getPerms();
   if ($dataset)
     return ($this->ops[$op] && in_array($dataset,$this->ops[$op]));
   else
     return ($this->ops[$op]);
  }
  
  function operations($dataset=0)
  {
   $this->getPerms();
   if ($dataset)
     return ($this->datasets[$dataset]);
   else
     return (array_keys($this->ops));
  }
  
  function datasets($op=0)
  {
   $this->getPerms();
   if ($op)
     return ($this->ops[$op]);
   else
     return (array_keys($this->datasets));
  }
  
 }// class UserPermissions

?>
