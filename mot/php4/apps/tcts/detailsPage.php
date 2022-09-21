<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for manipulating the transaction details page.
 // 
 // Created:		2001/01/14
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/02/05 16:16:26  eganp
 // History:		Added new field - open_date
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");
 
 class DetailsPage extends Page
 {
  var $classname = 'DetailsPage';
  
  function DetailsPage(&$context)
  {
   $this->Page('details.xhtml',$context);
  }

  function parsePage(&$trans,$msg=0)
  {
   $perm = &$this->context['userPerms'];
   $status = $trans->details['status'];
   
   if ($msg) $this->showMessage($msg);
   
   foreach (array('trans_id','batch_id','prev_id','employee','employee_name',
   		'type','status','status_name','center','last_proc_by',
		'last_proc_by_name','submitted_by','submitted_by_name',
		'trans_date','effective_date','approved_date','open_date','close_date',
		'submitted_via','reject_reason') as $var)
     $this->set_var($var,$trans->details[$var]);

   if (!$perm->canDoOp('update') || $status=='Rejected'){
     $this->remove_block('reject_form');
     $this->remove_block('reject_reason_form');
   }else{
     $this->parseList('reject_reason',getList('reject_reason',$this->context['center'],$this->context['db']),$f['reject_reason']);
   }
   
   if (!$perm->canDoOp('update') || $status!='Open')
     $this->remove_block('close_form');
   
   if (!$perm->canDoOp('update') || $status!='Closed')
     $this->remove_block('reopen_form');
   
   if (!$perm->canDoOp('update') || $status!='Pending')
     $this->remove_block('integrate_form');
   
   if (!$perm->canDoOp('delete') || $status=='Deleted')
     $this->remove_block('delete_form');

  }
  
 }// class DetailsPage

?>
