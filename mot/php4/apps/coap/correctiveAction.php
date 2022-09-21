<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class encapsulating operations on corrective_action table.
 // 
 // Created:		2001/01/31
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.1  2001/02/01 16:30:54  eganp
 // History:		Simple systax conversion of js files from version 1
 // History:		  Yet to be even parsed.  To do -
 // History:		
 // History:		Display of correct template determined in main
 // History:		Move of code from original html files to main
 // History:		Convert db handling from cursors to PHP db.
 // History:		$form (context['form']) and $db (context['db']) need to be
 // History:		 passed around.  Originally SSJS database & request objects
 // History:		 were global.
 // History:		Client side funcs need to be stored in client js file.
 // History:		Move from prints to return of html string to be inserted in
 // History:		 to templates
 // History:
 // 
 ///////////////////////////////////////////////////////////////////////////////

 class CorrectiveAction extends CoapTable
 
  function CorrectiveAction()
  {
   CoapTable('corrective_action');

   // This is just too cool not to do!!
   // eval(StandardTable_writeUpdateForm.toString().replace(/StandardTable/,"CoapTable").replace(/this\.columns\[c\]\.title/,"(this.columns[c].name.match(/_cid/)?(\"<A HREF=\\\"javascript:null;\\\" onClick=\\\"window.open('empSearch.html?targetField=\"+this.columns[c].formName+\"&initial=\"+this.columns[c].formName+\"+is+not+null','empSearch','width=250,height=350,resizable=1,scrollbars=yes'); return false;\\\">\"+this.columns[c].title+\"</A>\"):this.columns[c].title)"));
   // this.writeUpdateForm = CoapTable_writeUpdateForm;

  }// CorrectiveAction


  // Overrides the standard getRows() to do a join to the emp_ident table
  // in order to print names instead of CIDs in the record table.
  function getRows(&$db,&$form,$whereClause,$order)
  {
   if ($order)
     return ($db->query("SELECT NVL(tl.name,team_leader_cid) AS team_leader_cid,NVL(hr.name,hr_cid) AS hr_cid,ca.rec,ca.cid,ca.start_date,end_date,ca.reason,ca.stage,ca.interview_date,ca.review_date,ca.outcome FROM corrective_action ca, emp_ident tl, emp_ident hr WHERE ca.team_leader_cid = tl.cid(+) AND ca.hr_cid = hr.cid(+) AND ca.cid = '".$form['cid']."' ORDER BY ca.start_date"));
   else
     return ($db->query("SELECT * FROM ".$this->tableName.($whereClause?(" WHERE ".$whereClause):"").($order?(" ORDER BY ".$order):"")));
  }// getRows
  
 }
 
?>
