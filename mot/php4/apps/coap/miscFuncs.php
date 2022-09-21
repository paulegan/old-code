<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	A few miscellaneous funcs.
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


 // Simply adds "hours" to the end of the string.
 function mungeDuration($d)
 {
  return ($d." hours");
 }// mungeDuration


 // Converts "Certification" to "No" and "Re-certification" to "yes"
 function mungeRecert($r)
 {
  return (preg_match("/^recert/i",$r)?"Yes":"No");
 }// mungeRecert


 // Called by the TabForm object to generate the HTML for each
 // of the 3 COAP tabs.  First prints the CID select form generated
 // by EmpIdent, then displays a table of records for the selected
 // employee followed by a button to add a new record. 
 // Note: corrective action has its own prototype.
 function writeCoapTab()
 {
  $empIdent = new EmpIdent();
  if ($form['tab'] == "corrective_action")
    $ct = new CorrectiveAction();
  else
    $ct = new CoapTable($form['tab']);

  $form['cid'] = $empIdent->writeSelectForm($form['cid']);

  print("<BR CLEAR=ALL>\n");

  if ($form['cid']) {

    if ($form['update'] && !$ct->update($form))
      print("<P><FONT SIZE=+1 COLOR=RED>Record update failed! - ".$ct->errorMessage."</FONT></P>\n");

    $ct->writeTable("cid = '".$form['cid']."'","start_date",true);

    print("<SCRIPT LANGUAGE=\"JavaScript\">\n <!--\n\n  window.name = \"coap_main\";\n // -->\n</SCRIPT>\n");
    print("<FORM>\n");
    print(" <TABLE BORDER=0 WIDTH=90% CELLSPACING=0 CELLPADDING=0><TR><TD ALIGN=RIGHT><INPUT TYPE=SUBMIT VALUE=\" Add New Record \" onClick=\"window.open('update.html?tab=".$form['tab']."&cid=".$form['cid']."','coap_update','width=600,height=140,resizable=1,scrollbars=yes'); return false;\"></TD></TR></TABLE>\n");
    print("</FORM>\n");
  }

  return (true);
 }// writeCoapTab


 // Writes out a form for selecting which table to admin on the
 // admin tab.  Uses project.adminTables.
 function writeAdminTableSelectForm()
 {
  global $ADMIN_TABLES;
  
  print(" <FORM METHOD=POST NAME=\"tableSelect\">\n");
  print(" <TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
  print("  <TABLE BORDER=0 WIDTH=100%><TR>\n   <TD ALIGN=LEFT>&nbsp;</TD>\n");
  print("   <TD ALIGN=LEFT><FONT FACE=\"Arial,Helvetica\">Select table to admin:</FONT></TD>\n");
  print("   <TD ALIGN=RIGHT><CENTER>\n");
  print("    <SELECT NAME=\"table\" onChange=\"tableSelect.submit();\">\n");

  for ($t=0; $t<count($ADMIN_TABLES); $t++)
    print("     <OPTION VALUE=\"".$ADMIN_TABLES[$t]->name."\" ".(($form['table']==$ADMIN_TABLES[$t]->name)?"SELECTED":"")."> ".$ADMIN_TABLES[$t]->title."\n");

  print("    </SELECT>\n");
  print("   </CENTER></TD>\n  </TR></TABLE>\n");
  print(" </TD></TR></TABLE>\n");
  print(" </FORM>\n\n");
  print(" <BR CLEAR=ALL>\n");

  return (true);
 }// writeAdminTableSelectForm


 // Function called by TabForm to generate the body of the admin tab.
 // There are two cases - the user access table was selected or one
 // of the pick-lists was selected.  For the user access table there
 // are two forms printed - one to update the current records & one to
 // make an addition.  For the pick-lists, the current list is displayed
 // followed by an add form.
 function writeAdminTab()
 {
  writeAdminTableSelectForm();

  if (!$form['table'] || $form['table']=="user_access") {
    if ($form['update'] && $form['update']=="all" && !$userAccess->updateAll($form))
      print("<P><FONT SIZE=+1 COLOR=RED>User update failed! - ".$userAccess->errorMessage."</FONT></P>\n");

    if ($form['update'] && $form['update']=="add" && !$userAccess->update(request))
      print("<P><FONT SIZE=+1 COLOR=RED>User add failed! - ".$userAccess->errorMessage."</FONT></P>\n");

    $userAccess->writeUpdateAllForm();
    print(" <BR CLEAR=ALL>\n");
    $userAccess->writeUpdateForm();
  }else{
    $pickList = new PickList($form['table']);

    if ($form['add'] && !$pickList->update($form))
      print("<P><FONT SIZE=+1 COLOR=RED>Pick-list addition failed! - ".$pickList->errorMessage."</FONT></P>\n");

    print("<FORM>\n");
    $pickList->writeSelectList("","",true);
    print("</FORM>\n\n<BR CLEAR=ALL>\n\n");
    $pickList->writeUpdateForm();
  }

  return (true);
 }// writeAdminTab

?>
