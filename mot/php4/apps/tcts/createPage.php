<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for manipulating the transaction create form page.
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
 // History:		Revision 1.3  2001/02/05 19:03:59  eganp
 // History:		Added in check_cid warning and tidied up the rest
 // History:
 // History:		Revision 1.2  2001/01/24 19:18:20  eganp
 // History:		Now sets batch ID & employee name.
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");
 
 class CreatePage extends Page
 {
  var $classname = 'CreatePage';
  var $lastSet = array();
  var $page;
  
  function CreatePage(&$context)
  {
   $this->page = ($context['form']['_op']=='submit batch')?'createBatch.xhtml':'create.xhtml';
   $this->Page($this->page,$context);
   $this->lastSet['effective_day'] = date('d');
   $this->lastSet['effective_month'] = date('m');
   $this->lastSet['effective_year'] = date('Y');
   $this->lastSet['approved_day'] = date('d');
   $this->lastSet['approved_month'] = date('m');
   $this->lastSet['approved_year'] = date('Y');
  }

  
  function parsePage($msgs=0)
  {
   $f = &$this->context['form'];
   
   if (($f['_stage']=='commit') && ($msgs[0]=='create_success'))
     $f = array('num_created' => $f['num_lines'],
     		'submitted_by' => $f['submitted_by'],
		'batch_id' => $f['batch_id']);	// Clear most form vars
   
   $f['num_lines'] = ($f['num_lines']>0)?$f['num_lines']:1;

   if ($msgs) foreach ($msgs as $msg) $this->showMessage($msg);
   
   if ($this->page=='createBatch.xhtml')
     $this->parseList('submitted_via',getList('submission_type',$this->context['center'],$this->context['db']),$f['submitted_via']);
   else
     $this->parseList('submitted_by',$this->context['appPerms']->usersWithOp('submit batch',$this->context['center']),$f['submitted_by']);
   
   if (!$f['trans_day'])
     list($f['trans_day'],$f['trans_month'],$f['trans_year'])=split(' ',date('d m Y'));

   foreach (array('num_lines','batch_id','employee_err_line','effective_err_line',
                  'approved_err_line','num_created','trans_day','trans_month',
		  'trans_year') as $var)
     $this->set_var($var,$f[$var]);

   $this->set_block($this->root(), 'employee_row','employee_row_block');
   $this->set_block('employee_row', 'type_options','type_options_block');
   $this->set_message('employee_row','unknown_cid','unknown_cid_msg');
   for ($n=1; $n<=$f['num_lines']; $n++){
     $this->set_var('n',$n);
     $this->set_var('employee',$f['employee'.$n]);
     $this->set_var('employee_name',$f['employee_name'.$n]);
     if ($f['_stage']=='check cids' && !$f['employee_name'.$n])
       $this->set_var('unknown_cid',$this->get_var('unknown_cid_msg'));
     else
       $this->set_var('unknown_cid',"");
     foreach (array("effective_","approved_") as $var){
       $this->set_var($var."day",$this->getLastSet($var."day",$f[$var."day".$n]));
       $this->set_var($var."month",$this->getLastSet($var."month",$f[$var."month".$n]));
       $this->set_var($var."year",$this->getLastSet($var."year",$f[$var."year".$n]));
     }
     $this->set_var('type_options_block', '');
     foreach (getList('type',$this->context['center'],$this->context['db']) as $type){
       $this->set_var('type_value',$type);
       $this->set_var('type_name',$type);
       $this->set_var('type_selected',($type==$this->getLastSet('type',$f['type'.$n]))?'selected="selected"':"");
       $this->parse('type_options_block', 'type_options', true);
     }
     $this->parse('employee_row_block', 'employee_row', true);
   }

  }
  
  function getLastSet($field,$var)
  {
   return ($this->lastSet[$field]=($var?$var:$this->lastSet[$field]));
  }
  
 }// class CreatePage

?>
