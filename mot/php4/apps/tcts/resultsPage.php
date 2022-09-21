<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for manipulating the search results page.
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
 // History:		Revision 1.3  2001/01/25 20:35:09  eganp
 // History:		Added Easter Egg
 // History:
 // History:		Revision 1.2  2001/01/24 19:27:52  eganp
 // History:		Added handling of result limit
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");
 
 class ResultsPage extends Page
 {
  var $classname = 'ResultsPage';

  function ResultsPage(&$context)
  {
   $this->Page('results.xhtml',$context);
  }
  
  function parsePage(&$results,$msg=0)
  {
   $f = &$this->context['form'];

   foreach ($results as $record){
     foreach (array('trans_id','batch_id','employee','submitted_by',
     		'type','trans_date','status') as $var)
       $this->set_var($var,$record[$var]?$record[$var]:"&nbsp;");
     $this->set_var('row_class',((++$r)%2)?"odd":"even");
     $this->parse_block('result_row');
   }

   if ($msg) $this->showMessage($msg);

   if (!$this->context['userPerms']->canDoOp('update') || !$f['pending_batch']){
     $this->remove_block('integrate_form');
     $this->remove_block('reject_form');
     $this->remove_block('reject_reason_form');
   }else{
     $this->set_var('pending_batch',$f['pending_batch']);
     $this->parseList('reject_reason',getList('reject_reason',$this->context['center'],$this->context['db']),$f['reject_reason']);
   }
   
   if ($f['total']==42)
     $this->set_var('total_egg','<a target="_blank" href="http://www.everything2.com/index.pl?node=42">42</a>');
   else
     $this->set_var('total_egg',$f['total']);
   
   if ($f['total']==RESULTS_LIMIT)
     $this->showMessage('max_reached');
     
   if ($f['start']==1)
     $this->remove_block('prev_form');
     
   if ($f['end']>=$f['total'])
     $this->remove_block('next_form');
     
   $this->set_var(array('batch_id' => $f['batch_id'],
   	   		'start' => $f['start'],
   	   		'end' => $f['end'],
   	   		'total' => $f['total'],
   	   		'next' => $f['start']+RESULTS_PER_PAGE,
   	   		'prev' => $f['start']-RESULTS_PER_PAGE,
   	   		'searchClause' => htmlspecialchars($f['searchClause'])));
  }
  
 }// class ResultsPage

?>
