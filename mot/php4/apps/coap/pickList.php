<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Pick-list class slightly modifies a standard table.
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
 // History:		Revision 1.1  2001/02/01 16:30:55  eganp
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

 class PickList extends StandardTable
 {
 
  function PickList(name)
  {
   global $TABLE_DEF;
   StandardTable($name,$TABLE_DEF[$name]);
  }// PickList


  // Writes a single or multiple select list called "formName" with 
  // the option matching "sel" selected. e.g. call - writeSelectList("name","",true);
  function writeSelectList(&$db,$formName,$sel,$multi)
  {
   $db->query("SELECT name FROM ".$this->tableName);

   print("<SELECT NAME=\"".$formName."\"".$multi?"MULTIPLE SIZE=\"30\">\n":"");

   while ($db->next_record())
     print("  <OPTION".(($db->f('NAME')==$sel)?" SELECTED":"")."> ".$db->f('NAME')."\n");
 
   print("</SELECT>\n");

   return (true);
  }// writeSelectList


  // Overrides the standard writeUpdateForm() in favour of a simpler
  // one with one text field and a submit button.
  function writeUpdateForm()
  {
   $this->writeClientCheck("validateUpdateForm");

   print("  <FORM METHOD=POST onSubmit=\"return validateUpdateForm(this);\">\n");
   print("  <INPUT TYPE=HIDDEN NAME=\"table\" VALUE=\"".$form['table']."\">\n");
   print("  <INPUT TYPE=HIDDEN NAME=\"add\" VALUE=\"true\">\n");
   print("  <TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
   print("   <TABLE WIDTH=100% BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>\n");
   print("     <TD ALIGN=LEFT><INPUT TYPE=TEXT SIZE=20 NAME=\"name\"></TD>\n");
   print("     <TD ALIGN=RIGHT><INPUT TYPE=SUBMIT VALUE=\" Add New Entry \"></TD>\n");
   print("   </TR></TABLE>\n");
   print("  </TD></TR></TABLE>\n");
   print("  </FORM>\n");

   return (true);
  }// writeUpdateForm
 
 }
 
?>
