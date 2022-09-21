<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for dealing with the user edit page.
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
 // History:		Revision 1.2  2001/01/24 19:32:34  eganp
 // History:		When only 1 center center_name becomes "Access"
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");

  
 class UserEditPage extends Page
 {
  var $classname = 'UserEditPage';

  function UserEditPage(&$context)
  {
   $this->Page('userEdit.xhtml',$context);
  }

  function parsePage($msg=0)
  {
   $centers = $this->context['userPerms']->datasets('manage users');
   if ($msg) $this->showMessage($msg);
   
   $perms = $this->context['appPerms']->all($centers);
   foreach ($perms as $uid => $data){
     $p = "";
     foreach ($data['ops'] as $op => $d)
       $p .= join(":".$op."','",$d).":".$op."','";
     $this->set_var('user_id',$uid);
     $this->set_var('user_selected',($this->context['form']['user']==$uid)?'selected="selected"':'');
     $this->set_var('user_perms',$p);
     $this->set_var('user_name',$uid." - ".$data['name']);
     $this->parse_block('user_options');
     $this->parse_block('js_perms');
   }
   
   foreach ($centers as $center){
     $this->set_var('center',$center);
     $this->set_var('center_name',(count($centers)>1)?$center:"Access");
     $this->parse_block('perm_matrix');
     $this->parse_block('all_perms');
   }

  }
  
  
 }// class UserEditPage

?>
