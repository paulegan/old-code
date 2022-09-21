<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class for dealing with COAP type tables.
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

 class CoapTable extends StandardTable
 {
  var $clientFuncs = array();
  var $updateFormParams;
  var $updateFormSubmitCode;
  
  function CoapTable($name)
  {
   global $TABLE_DEF;
   StandardTable($name,$TABLE_DEF[$name]);

   $this->clientFuncs[] = 'looksLikeDate';
   $this->clientFuncs[] = 'looksLikeCID';
   $this->updateFormParams = 'method="post" action="/coap/" target="coap_main"';
   $this->updateFormSubmitCode = ' <input type="button" value=" Cancel " onclick="window.close();"><input type="button" value=" Update " onclick="if ( validateUpdateForm(this.form) ) { this.form.submit(); window.close(); }">';

  }// CoapTable


  // Adds additional server side checks for the date and CID fields in
  // the COAP tables.  Returns false if the date fields are not valid
  // dates or if the CIDs do not match /[0-9]{8}/
  function validDataAuxCheck($form)
  {
   for ($c=0; $c<count($this->columns); $c++){
     $col = $this->columns[$c];
     if ( $col->type=="date" && $form[$col->formName] && !validDate($form[$col->formName])){
       $this->errorMessage = "The ".$col->title." field should be of the form DD-MON-YYYY.";
       return (false);
     }
     if ( preg_match("/cid$/",$col->name) && $form[$col->formName] && !looksLikeCID($form[$col->formName]) ){
       $this->errorMessage = "The ".$col->title." field is not a valid CID.";
       return (false);
     }
   }

   return (true);
  }// validDataAuxCheck


  // Adds additional client side checks for the date and CID fields in
  // the COAP tables.  Returns false if the date fields are not of the form
  // DD-MON-YYYY or if the CIDs do not match /[0-9]{8}/
  function writeAuxClientCheck()
  {
   for ($c=0; $c<count($this->columns); $c++){
     $col = $this->columns[$c];
     if ( $col->type=="date" ){
       print("  if (".($col->canBeNull?"form.".$col->formName.".value!=\"\" && ":"")."!looksLikeDate(form.".$col->formName.".value)){\n");
       print("    alert(\"The ".$col->title." field should be of the form DD-MON-YYYY!\");\n");
       print("    return (false);\n");
       print("  }\n\n");
     }
     if ( $col->name.match(/cid$/) ){
       print("  if (".($col->canBeNull?"form.".$col->formName.".value!=\"\" && ":"")."!looksLikeCID(form.".$col->formName.".value)){\n");
       print("    alert(\"The ".$col->title." field is not a valid CID.\");\n");
       print("    return (false);\n");
       print("  }\n\n");
     }
   }

   return (true);
  }// writeAuxClientCheck


  // Updates the $form object with additional properties - converts
  // each date string to a date object and adds rec number from the
  // respective sequence if no rec number is given.  Returns the
  // modified $form object.
  function augmentRequest(&$form,&$db)
  {
   if ($this->requestAugmented) return ($form);

   // for ($c=0; $c<count($this->columns); $c++){
   //   $col = $this->columns[$c];
   //   if ( $col->type=="date" ){
   //     if ($form[$col->formName] && preg_match("/^[0-9]{2}-[A-Za-z]{3}-[0-9]{4}$/",$form[$col->formName])){
   //       $dateElements = split("-",$form[$col->formName]);
   //       $form[$col->formName] = new Date(dateElements[1].", ".dateElements[0]." ".dateElements[2]);
   //     }
   //   }
   // }

   if (!$form['rec'])
     $form['rec'] = $db->next_seq($this->tableName.'_rec');

   $this->requestAugmented = true;

   return ($form);
  }// augmentRequest


  // Overrides standard sub-method used by writeUpdateForm() to generate
  // each form element. Writes a pick-list for each of the list fields
  // and munges the default dates to DD-MON-YYYY.
  function writeFormElement($col,$val)
  {
   $colName = strtoupper($col->name);

   if ($col->type=="list"){
     $pickList = new PickList($COL_2_TABLE[$this->tableName.".".$col->name]);
     $pickList->writeSelectList($col->formName,$val);
   }else{
     print('<input type="text" name="'.$col->formName.'" size="'.$col->length.'" value="'.$val."\" />\n");
   }

   return (true);
  }// writeFormElement


  // Overrides standard sub-method used by writeUpdateForm() to write
  // hidden form values - adds a dump of the $form object in order
  // to maintain state - in particular the rec field.
  function writeHiddenFormValues(&$form,$rec)
  {
   if ($rec){
     for ($c=0; $c<count($this->columns); $c++)
       if (!$this->columns[$c]->display)
	 print(' <input type="hidden" name="'.$this->columns[c]->formName.'" value="'.$rec[strtoupper($this->columns[$c]->name)]."\" />\n");
   }

   foreach ( $form as $item )
     print(' <input type="hidden" name="'.$item.' value="'.$form[$item]."\" />\n");

   return (true);
  }// writeHiddenFormValues


  // Overrides standard sub-method used by writeTable() to generate the
  // link for updating a record.  Writes a link to open a new window with
  // an update form.
  function editLinkStr(&$form,$row)
  {
   return ("<a href=\"javascript:null;\" onclick=\"window.open('update.html?tab=".$form['tab']."&amp;row=".$row['REC']."','coap_update','width=500,height=140,resizable=1,scrollbars=yes'); return false;\"><img src=\"images/pen.gif\" border=\"0\" width=\"34\" height=\"19\" /></a>");
  }// editLinkStr

 }
 
?>
