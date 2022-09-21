<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	The transaction class encapsulates all opertions to
 //			be performed on a transaction.
 // 
 // Created:		2001/01/10
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.3  2001/02/05 16:16:26  eganp
 // History:		Added new field - open_date
 // History:
 // History:		Revision 1.2  2001/01/24 19:30:45  eganp
 // History:		On create batch ID & trans IDs are now stored in context
 // History:		Added limit on number of results returned
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////
 
 require_once("miscFuncs.php");

 class Transaction
 {
  var $classname = 'Transaction';
  var $details = array();
  var $context;
  
  function Transaction(&$context,$id=0)
  {
   $this->context = &$context;
   if ($id) $this->getDetails($id);
  }

  function getDetails($id)
  {
   if ($this->details) return ($this->details);
   
   $selectStmt =
     " SELECT DISTINCT t.trans_id, t.batch_id, t.prev_id,".
     "   t.employee, INITCAP(twin.full_name) AS employee_name,".
     "   t.type, t.status, s.name AS status_name, t.center,".
     "   t.last_proc_by, NVL(lpb1.user_name,INITCAP(lpb2.full_name)) AS last_proc_by_name,".
     "   t.submitted_by, NVL(sb1.user_name,INITCAP(sb2.full_name)) AS submitted_by_name,".
     "   t.trans_date, t.effective_date, t.approved_date, t.open_date,".
     "   t.close_date, t.submitted_via, t.reject_reason".
     " FROM trans t, status s, perm lpb1, perm sb1,".
     "      all_twin twin, all_twin lpb2, all_twin sb2".
     " WHERE t.employee = twin.cid (+)".
     "   AND t.last_proc_by = lpb1.user_id (+)".
     "   AND t.last_proc_by = lpb2.cid (+)".
     "   AND t.submitted_by = sb1.user_id (+)".
     "   AND t.submitted_by = sb2.cid (+)".
     "   AND t.status = s.code AND s.center = '".$this->context['center']."'".
     "   AND t.trans_id = '$id'";
   
   $db = &$this->context['db'];
   $db->query($selectStmt);
    if ($db->next_record())
     $this->details = $db->Record;
   
   return ($this->details);
  }
  
  function create($status)
  {
   $f = &$this->context['form'];
   $db = &$this->context['db'];
   $msgs = array();
   
   $msgs = $this->validateData();
   
   if (!$msgs){		// If there were no errors, complete insertion
     $db->beginTransaction();
     $f['batch_id'] = BATCH_ID_PREFIX.$db->nextid("batch_id_seq");
     for ($n=1; $n<=$f['num_lines']; $n++){
       $f['trans_id'.$n] = TRANS_ID_PREFIX.$db->nextid("trans_id_seq");
       $db->query("INSERT INTO trans VALUES ('".$f['trans_id'.$n].
                  "','".$f['batch_id']."',NULL,'".$f['employee'.$n]."','".$f['type'.$n].
		  "','".$status."','".$this->context['center']."','".$this->context['user']->uid().
		  "','".($f['submitted_by']?$f['submitted_by']:$this->context['user']->uid()).
		  "','".$f['trans_date']."','".$f['effective_date'.$n]."','".$f['approved_date'.$n].
		  "',NULL,'".$f['submitted_via']."',NULL,".
		  (($status=='Open')?"SYSDATE":"NULL").")");
       if ($db->num_rows()!=1){
         $db->rollback();
         $this->context['eh']->halt("Unknown error when inserting transactions!");
       }
     }
     $db->commit();
     $msgs[] = 'create_success';
   }
   
   return ($msgs);
  }// create
  
  function validateData()
  {
   $f = &$this->context['form'];
   $msgs = array();
   
   if (($f['trans_date']=oraDate($f['trans_day'],$f['trans_month'],$f['trans_year']))=='invalid')
     $msgs[] = 'invalid_trans_date';
   
   for ($n=1; $n<=$f['num_lines']; $n++){
     if (!preg_match('/^\w{3,8}$/',$f['employee'.$n])){
       addErrLineNo($f['employee_err_line'],$n);
       $msgs[] = 'invalid_employee';
     }
     if (($f['approved_date'.$n]=oraDate($f['approved_day'.$n],$f['approved_month'.$n],$f['approved_year'.$n]))=='invalid'){
       addErrLineNo($f['approved_err_line'],$n);
       $msgs[] = 'invalid_approved_date';
     }
     if (($f['effective_date'.$n]=oraDate($f['effective_day'.$n],$f['effective_month'.$n],$f['effective_year'.$n]))=='invalid'){
       addErrLineNo($f['effective_err_line'],$n);
       $msgs[] = 'invalid_effective_date';
     }
   }
   
   return ($msgs);
  }
  
  function update($id,$status,$reject_reason=0)
  {
   $db = &$this->context['db'];
   
   switch ($status){
     case 'Rejected':
       if (!$reject_reason)
         $this->context['eh']->halt("Reject reason not specified!");
       else
         $setRejectReason = ", reject_reason = '$reject_reason'";
     case 'Closed':
       $setCloseDate = ", close_date = SYSDATE";
       break;
     case 'Open':
       $setOpenDate = ", open_date = SYSDATE";
   }
   
   $db->beginTransaction();
   $db->query("UPDATE trans SET last_proc_by='".$this->context['user']->uid().
              "', status = '$status' $setCloseDate $setOpenDate $setRejectReason WHERE ".
	      (preg_match("/^".BATCH_ID_PREFIX."/",$id)?"batch_id":"trans_id")." = '$id'");
   if ($db->num_rows()<1){
     $db->rollback();
     $this->context['eh']->halt("Unknown error when updating transaction!");
   }
   
   return ($db->commit());
  }
  
  function reopen($id)
  {
   $db = &$this->context['db'];

   $db->beginTransaction();
   $new_id = TRANS_ID_PREFIX.$db->nextid("trans_id_seq");
   $db->query("INSERT INTO trans SELECT '".$new_id."', batch_id, trans_id, ".
              "employee, type, 'Open', center, '".$this->context['user']->uid().
	      "',submitted_by, SYSDATE, effective_date, approved_date, NULL,".
	      "submitted_via,NULL,SYSDATE FROM trans WHERE trans_id = '$id'");
   if ($db->num_rows()!=1){
     $db->rollback();
     $this->context['eh']->halt("Unknown error when updating transaction!");
   }
   $db->commit();
   
   return ($new_id);
  }
  
  function find(&$msg)
  {
   $f = &$this->context['form'];
   $db = &$this->context['db'];
   $results = array();

   $f['start'] = $f['start']?$f['start']:1;   
   if ($f['start']<1) $this->context['eh']->halt("Invalid starting record for transaction search");

   $f['pending_batch'] = $f['batch_id'];
   
   if (!$f['searchClause']){
     $f['searchClause'] = "WHERE rownum <= ".RESULTS_LIMIT." AND status <> 'Deleted' AND ".
       "center IN ('".join("','",$this->context['userPerms']->datasets('view'))."') ";
     if (!($f['searchClause'] = buildSearchClause($f['searchClause'],$f,$msg)))
       return (0);	// Error building search clause
   }
     
   $db->query("SELECT * FROM trans ".$f['searchClause']);

   // If this is the first search on this query run through all matches, get a
   // count & check if this is a pending batch.  Otherwise just return those
   // results between start & end.
   if (!$f['total']){
     $f['end'] = $f['start']+RESULTS_PER_PAGE-1;
     while ($db->next_record()){
       if ((++$r)<=$f['end'])
         $results[] = $db->Record;
       if ($db->Record['status']!='Pending')
         $f['pending_batch'] = 0;
     }
     $f['total'] = $r;
     if ($f['end']>$f['total']) $f['end']=$f['total'];
   }else{
     $f['end'] = (($f['start']+RESULTS_PER_PAGE-1)>$f['total'])?$f['total']:($f['start']+RESULTS_PER_PAGE-1);
     for ($r=1; $r<$f['start']; $r++){
       $db->next_record();
     }
     for ($r=$f['start']; $r<=$f['end']; $r++){
       $db->next_record();
       $results[] = $db->Record;
     }  
   }

   return ($results);
  }// find
  
 }// class Transaction
 
?>
