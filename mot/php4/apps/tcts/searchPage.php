<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Object for manipulating the search page.
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
 // History:		Revision 1.2  2001/01/24 19:29:28  eganp
 // History:		Removes center list if only one is accessable
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("page.php");
 
 class SearchPage extends Page
 {
  var $classname = 'SearchPage';

  function SearchPage(&$context)
  {
   $this->Page('search.xhtml',$context);
  }

  function parsePage($msg=0)
  {
   $f = &$this->context['form'];
   
   foreach (array('trans_id','batch_id','employee','trans_day',
   		'trans_month','trans_year') as $var)
     $this->set_var($var,$f[$var]);
   
   $centers = $this->context['userPerms']->datasets('view');
   if (count($centers)>1)
     $this->parseList('center',$centers,$f['center']);
   else
     $this->remove_block('center_select');
     
   $this->parseList('status',
   		array_delete(array('Deleted'),statusList($this->context)),
		$f['status'],0);
   $this->parseList('submitted_by',
   		$this->context['appPerms']->usersWithOp('submit batch',$this->context['center']),
		$f['submitted_by']);
   
   if ($msg) $this->showMessage($msg);
  }
  
 }// class SearchPage

?>
