<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for dealing with the user add page.
 // 
 // Created:		2001/01/15
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/01/24 19:32:09  eganp
 // History:		Now sets role
 // History:		When only 1 center center_name becomes "Access"
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");

  
 class UserAddPage extends Page
 {
  var $classname = 'UserAddPage';
  
  function UserAddPage(&$context)
  {
   $this->Page('userAdd.xhtml',$context);
  }

  function parsePage($msg=0)
  {
   $f = &$this->context['form'];
   $centers = $this->context['userPerms']->datasets('manage users');
   if ($msg) $this->showMessage($msg);
   
   $this->set_var('users',$f['users']);
   $this->set_var('role',$f['role']?$f['role']:'none');
   
   foreach ($centers as $center){
     $this->set_var('center',$center);
     $this->set_var('center_name',(count($centers)>1)?$center:"Access");
     $this->parse_block('perm_matrix');
     $this->parse_block('role_all');
     $this->parse_block('role_standard');
     $this->parse_block('role_admin');
     $this->parse_block('role_hr');
   }

  }
  
 }// class UserAddPage

?>
