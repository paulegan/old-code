<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for dealing with the lists page.
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
 // History:		Revision 1.2  2001/01/24 19:19:01  eganp
 // History:		Fixed bug with list selection
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");

  
 class ListEditPage extends Page
 {
  var $classname = 'ListEditPage';

  function ListEditPage(&$context)
  {
   $this->Page('listEdit.xhtml',$context);
  }

  function parsePage($msg=0)
  {
   if ($msg) $this->showMessage($msg);
   
   if ($this->context['form']['listName']=='reject_reason'){
     $list = getList('reject_reason',$this->context['center'],$this->context['db']);
     $this->set_var('reject_reason_selected','selected="selected"');
     $this->set_var('submission_type_selected','');
     $this->set_var('type_selected','');
     $this->set_var('listName','reject_reason');
   }elseif ($this->context['form']['listName']=='submission_type'){
     $list = getList('submission_type',$this->context['center'],$this->context['db']);
     $this->set_var('reject_reason_selected','');
     $this->set_var('submission_type_selected','selected="selected"');
     $this->set_var('type_selected','');
     $this->set_var('listName','submission_type');
   }else{
     $list = getList('type',$this->context['center'],$this->context['db']);
     $this->set_var('reject_reason_selected','');
     $this->set_var('submission_type_selected','');
     $this->set_var('type_selected','selected="selected"');
     $this->set_var('listName','type');
   }
    
   foreach ($list as $item){
     $this->set_var('list_value',$item);
     $this->parse_block('list_options');
   }
   
  }
  
  
 }// class ListEditPage

?>
