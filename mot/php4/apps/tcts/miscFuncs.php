<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	A few miscellaneous functions for use with TCTS.
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
 // History:		Revision 1.3  2001/01/26 18:18:23  eganp
 // History:		Fixed bug with apostrophes in list entries
 // History:
 // History:		Revision 1.2  2001/01/24 19:25:12  eganp
 // History:		Fixed bug with date on search form
 // History:		Added mail notification
 // History:		Added function to check CIDs
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 function oraDate($day,$month,$year)
 {
  $months = array('JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC');
  if (!($day.$month.$year))
    return (0);
  elseif (($year<1900) || !checkdate($month,$day,$year))
    return ('invalid');
  else
    return ($day."-".$months[((int)$month)-1]."-".$year);
 }
 
 function getList($listName,$center,&$db)
 {
  static $list = array();
  
  if ($list[$listName][$center]) return ($list[$listName][$center]);

  $db->query("SELECT name FROM $listName WHERE center = '$center' ORDER BY name");
  while ($db->next_record())
    $list[$listName][$center][] = $db->f('name');

  return ($list[$listName][$center]);
 }
 
 function updateList($listName,&$db,$center,$listItems)
 {
  $db->beginTransaction();
  $db->query("DELETE FROM $listName WHERE center = '$center'");
  foreach ($listItems as $item){
    $db->query("INSERT INTO $listName VALUES ('". $db->add_specialcharacters($item)."','$center')");
    if ($db->num_rows()!=1){
      $db->rollback();
      $this->context['eh']->halt("Unknown error when updating list table!");
    }
  }
  $db->commit();
 }
  
 function statusList(&$context)
 {
  static $list = array();
  $db = &$context['db'];

  if ($list) return ($list);
  
  $db->query("SELECT code,name FROM status WHERE center = '".$context['center']."'");
  while ($db->next_record())
    $list[$db->f('code')] = $db->f('name');

  return ($list);
 }
 
 function buildSearchClause($searchClause,&$f,&$msg)
 {
  if ($f['trans_id']){
    if (!preg_match("/^T\w+$/",$f['trans_id'])){
      $msg = 'invalid_trans_id';
      return ("");
    }else{
      $searchClause .= "AND trans_id = '".$f['trans_id']."' ";
    }
  }
    
  if ($f['batch_id']){
    if (!preg_match("/^B\w+$/",$f['batch_id'])){
      $msg = 'invalid_batch_id';
      return ("");
    }else{
      $searchClause .= "AND batch_id = '".$f['batch_id']."' ";
    }
  }
    
  if ($f['employee']){
    if (!preg_match("/^\w+$/",$f['employee'])){
      $msg = 'invalid_employee';
      return ("");
    }else{
      $searchClause .= "AND employee = '".$f['employee']."' ";
    }
  }
  
  if ($f['trans_date']=oraDate($f['trans_day'],$f['trans_month'],$f['trans_year'])){
    if ($f['trans_date']=="invalid"){
      $msg = 'invalid_date';
      return ("");
    }else{
      $searchClause .= "AND trans_date >= '".$f['trans_date']."' ";
    }
  }
  
  if ($f['status']) $searchClause .= "AND status = '".$f['status']."' ";
  if ($f['submitted_by']) $searchClause .= "AND submitted_by = '".$f['submitted_by']."' ";
  if ($f['center']) $searchClause .= "AND center = '".$f['center']."' ";
 
  return ($searchClause);
 }// buildSearchClause
 
 function addErrLineNo(&$lineNos,$n)
 {
  if (!$lineNos){
    $lineNos = (string)$n;
  }else{
    $lineNos = ereg_replace(" &",",",$lineNos);		// Does this need to
    $lineNos .= " & $n";				// be i18n'sd??
  }
  
 }
 
 function array_delete($delThis, $fromThis)
 {
  $result = array();
  foreach ($fromThis as $key => $value)
    if (!in_array($value,$delThis))
      $result[$key] = $value;
  return ($result);
 }
 
 function getPermsFromMatrix(&$form)
 {
  $perms = array();
  foreach ($form as $name => $value)
    if (($value = 'on') && preg_match("/^([^:]*):([^:]*)$/",$name,$m))
      $perms[preg_replace("/_/"," ",$m[2])][] = preg_replace("/_/"," ",$m[1]);
  return ($perms);
 }
 
 function emailPendingNotification($mailDir,&$context)
 {
  global $HTTP_SERVER_VARS;
  
  $form = &$context['form'];
  
  $url = 'http://'.$HTTP_SERVER_VARS['HTTP_HOST'].$HTTP_SERVER_VARS['PHP_SELF'].
  		'?_op=view&_stage=results&batch_id='.$form['batch_id'];
		
  $templ = $mailDir."/pendingNotification.mail";
  if (!is_readable($templ)){
    $context['eh']->logError("Notification mail not sent - could not open $templ");
    return (false);
  }
  
  $mail = new Template($mailDir);
  $mail->set_file('mail',$templ);
  
  $mail->set_var(array(	'batch_id' => $form['batch_id'],
  			'submitted_by' => $context['user']->uid(),
  			'submitted_via' => $form['submitted_via'],
  			'trans_date' => $form['trans_date'],
  			'url' => $url));

  $mail->set_block('mail','transactions','t_block');
  for ($n=1; $n<=$form['num_lines']; $n++){
    $mail->set_var(array('trans_id' => $form['trans_id'.$n],
    			 'employee' => $form['employee'.$n],
  			 'type' => $form['type'.$n]));
    $mail->parse('t_block','transactions',true);
  }
  
  return (mail(	TXC_EMAIL_ADDR,
  		"Batch ".$form['batch_id']." pending integration to TCTS",
		$mail->subst('mail'),
		"From: ".TXC_EMAIL_ADDR));
 }
 
 function setNamesFromCIDs(&$form,&$db)
 {
  $cids = array();
  
  for ($n=1; $n<=$form['num_lines']; $n++){
    $db->query("SELECT INITCAP(full_name) AS name FROM all_twin WHERE ".
  	"cid = '".$form['employee'.$n]."'");
    if ($db->next_record())
      $form['employee_name'.$n] = $db->f('name');
  }
 }
 
 
 function notIndex($value)
 {
  return (is_string($value) || preg_match("/^[0-9]{8}$/",$value));
 }
 
?>
