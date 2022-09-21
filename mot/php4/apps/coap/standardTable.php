<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class for a standard table. Contains methods for operations on this table.
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

 class StandardTable
 {
  var $tableName;
  var $columns;
  var $key;
  var $keyFormName;
  var $requestAugmented;
  var $errorMessage;
  var $clientFuncs;
  
  function StandardTable($tableName,$columns,$key)
  {

   // Properties
   $this->tableName = $tableName || "dual";
   $this->columns = $columns;
   $this->key = $key || $this->columns[0]->name;
   $this->requestAugmented = false;
   $this->errorMessage = "";
   $this->clientFuncs = array();

   for ($c=0; $c<count($this->columns); $c++)
     if ($this->columns[$c]->name==$this->key)
       $this->keyFormName = $this->columns[$c]->formName;

  }// StandardTable


  // Returns a cursor for the specified rows in the specified order.
  function getRows(&$db,$whereClause,$order)
  {
   return ($db->query("SELECT * FROM ".$this->tableName.($whereClause?(" WHERE ".$whereClause):"").($order?(" ORDER BY ".$order):"")));
  }// getRows


  // Function to update one row (select by key) in the table with the
  // values given in the request object. If a row with the specified key
  // does not exist then a new row is created.  Will return false and set
  // errorMessage if anything goes wrong.
  function update(&$db,&$form)
  {
   $upd=false;

   if (!$this->validData($form))
     return (false);

   $db->beginTransaction();
   $db->query("SELECT * FROM ".$this->tableName." WHERE ".$this->key." = '".$form[$this->keyFormName]."'",true);

   if ($db->next_record())
     $upd=true;

   // record.assign(request,$this->columns);
   $this->setValues($form);
   
   // if ($upd)
   //   result = record.updateRow($this->tableName);
   // else
   //   result = record.insertRow($this->tableName);

   if ($db->num_rows()>1){
     $db->commit();
     return (true);
   }else{
     // $this->errorMessage = "Transaction rolled back! ".database.majorErrorMessage();
     $db->rollback();
     return (false);
   }

  }// update


  // Override this method if there are extra properties to be
  // added to the request object or properties are to be modified.
  function augmentRequest(&$form)
  {
   if ($this->requestAugmented) return ($form);

   // Add any additional properties to the request object here. e.g. - 
   // request[key] = project.Seq++;
   // $form['name'] = $form['name'].toUpperCase();

   $this->requestAugmented = true;

   return ($form);
  }// augmentRequest


  // Writes out a function that should be used in a form onSubmit
  // tag.  Returns false if any form element that has the validateOnClient
  // flag set and does not match the contraints in the columns array.
  // Use the clientFuncs array and the writeAuxClientCheck() method to
  // extend the functionality.
  function writeClientCheck($funcName)
  {
   $cols = $this->columns;

   print("<SCRIPT LANGUAGE=\"JavaScript\">\n");
   print(" <!--\n\n");

   // for ($f=0; $f<count($this->clientFuncs); $f++)
   //  print(" ".eval($this->clientFuncs[$f].".toString()")."\n");

   print(" function ".($funcName?$funcName:"validateUpdateForm")."(form)\n");
   print(" {\n");

   for ($c=0; $c<count($cols); $c++){
     if ($cols[$c]->validateOnClient){
       if (!$cols[c]->canBeNull){
	 print("  if (form.".$cols[$c]->formName.".value==\"\"){\n");
	 print("    alert (\"Please fill in the ".$cols[$c]->title." field.\");\n");
	 print("    return (false);\n");
	 print("  }\n");
       }
       if ($cols[$c]->type=="text"){
	 print("  if (form.".$cols[$c]->formName.".value.length>".$cols[$c]->length."){\n");
	 print("    alert (\"The ".$cols[$c]->title." field should be ".$cols[$c]->length." characters or less.\");\n");
	 print("    return (false);\n");
	 print("  }\n");
       }
       if ($cols[$c]->type=="number"){
	 print("  if (isNaN(form.".$cols[$c]->formName.".value)){\n");
	 print("    alert (\"The ".$cols[$c]->title." field does not contain a valid number.\");\n");
	 print("    return (false);\n");
	 print("  }\n");
       }
     }
   }

   $this->writeAuxClientCheck();

   print("  return (true);\n");
   print(" }\n");

   print("\n // -->\n");
   print("</SCRIPT>\n");

   return (true);
  }// writeClientCheck


  // Use to add extra client side checks.
  function writeAuxClientCheck()
  {
   // e.g. - 
   // print("  if (!form.name.value.match(/^[A-Z]*$/)){\n");
   // print("    alert (\"The name must contain only capital letters.\");\n");
   // print("    return (false);\n");
   // print("  }\n");

   return (true);
  }// writeAuxClientCheck


  // Returns false if any fields on the request object to not match
  // the constraints specified in the columns array.  The request object
  // is first augmented with the augmentRequest method.  Extend the
  // checks with the validDataAuxCheck() method.
  function validData(&$form)
  {
   $request = $this->augmentRequest($form);
   $cols = $this->columns;

   for ($c=0; $c<count($cols); $c++){
     $value = (request[$cols[$c]->formName]!="")?request[$cols[$c]->formName]:null;
     if ( !$cols[$c]->canBeNull && !$value ){
       $this->errorMessage = "Mandatory field, ".$cols[$c]->title.", not entered.";
       return (false);
     }
     if ( $cols[$c]->type=="text" && $value && strlen($value)>$cols[$c]->length ){
       $this->errorMessage = $cols[$c]->title." should be ".$cols[$c]->length." characters or less.";
       return (false);
     }
     if ( $cols[$c]->type=="number" && $value && isNaN($value) ){
       $this->errorMessage = $cols[$c]->title." is not a valid number!";
       return (false);
     }
   }

   if (!$this->validDataAuxCheck($form))
     return (false);

   return (true);
  }// validData


  // Used to add extra server side checks on the request properties.
  // Set $this->errorMessage if returning false.
  function validDataAuxCheck(request)
  {
   // e.g. - 
   // if (!$form['name'].match(/^[A-Z]*$/){
   //   $this->errorMessage = "The name must contain only capital letters.";
   //   return (false);
   // }

   return (true);
  }// validDataAuxCheck


  // Writes out an HTML table containing all the rows returned by
  // the whereClause and in the specified order.  Only columns whose
  // display flag is set are shown and values are run through the munge
  // function if it was given.  If incEditColumn is set then an
  // edit column is added, the contents of which is given by the
  // editLinkStr() method.
  function writeTable($whereClause,$order,$incEditColumn)
  {
   $this->getRows($whereClause,$order);

   print("<TABLE WIDTH=90% BORDER=1 CELLPADDING=2 CELLSPACING=0>\n");

   if (1>0){	//row count

     print("<TR BGCOLOR=#999999>\n");
     for ($c=0; $c<count($this->columns); $c++)
       if ($this->columns[$c]->display)
	 print("<TD><FONT FACE=\"Arial,Helvetica\">".$this->columns[$c]->title."</FONT></TD>\n");
     if ($incEditColumn)
       print("<TD><FONT FACE=\"Arial,Helvetica\">Edit</FONT></TD>\n");
     print("</TR>\n");

     while ($db->next_record()){
       print("<TR BGCOLOR=#FFFFFF>\n");
       for ($c=0; $c<count($this->columns); $c++){
	 $colName = $this->columns[$c]->name;
	 if ($this->columns[$c]->display)
 	   if ($this->columns[$c]->munge)
 	     print("<TD>".(($db->('colName'))?$this->columns[$c]->munge($db->('colName')):"&nbsp;")."</TD>");
 	   else
 	     print("<TD>".(($db->('colName'))?$db->('colName'):"&nbsp;")."</TD>");
       }
       if ($incEditColumn) print("<TD>".$this->editLinkStr(row)."</TD>\n");
       print("</TR>\n");
     }

   }else{
     print("<TR><TD ALIGN=CENTER>No records found.</TD></TR>\n");
   }

   print("</TABLE>\n");

   return (true);
  }// writeTable


  // This method is used to determine what should be printed in the edit
  // column of the table from writeTable().  Below is an example but this
  // method should almost certainly be overridden.
  function editLinkStr($row)
  {
   return ("<A HREF=\"".$PHP_SELF."?table=".$this->tableName."&".$this->keyFormName."=".$row[$this->key]."\">EDIT</A>");
  }// editLinkStr


  // Writes out an HTML form with an element for each column which should
  // be displayed.  If row is specified ( which should be a value from the
  // key column ) then the values from this row are used in the form.  If
  // defaultValues is given ( and row is not ) then properties of defaultValues
  // are matched to the form values. formParams can be used to override the
  // parameters in the FORM tag.  Similarily submitCode can be used to replace
  // the normal submit button.  If row is given but does not return a valid
  // row then the method returns false - but still displays a form. isUpdateFlag
  // is an optional name of a form element that should be set in order to
  // identify that an update should be done.  Override writeHiddenFormValues()
  // & writeFormElement() to extend functionality.
  function writeUpdateForm($row,$defaultValues,$formParams,$submitCode,$isUpdateFlag)
  {

   if ($row){
     $rec = $this->getRows($this->key." = '".$row."'");
     if (!$rec->next_record())
       $rec = null;
   }

   $this->writeClientCheck();

   print("<FORM ".($formParams?$formParams:"METHOD=POST onSubmit=\"return validateUpdateForm(this);\"").">\n");
   if ($isUpdateFlag) print(" <INPUT TYPE=HIDDEN NAME=".$isUpdateFlag." VALUE=\"true\" >\n");

   $this->writeHiddenFormValues($rec);

   print("<TABLE WIDTH=90% BORDER=0 CELLPADDING=2 CELLSPACING=0>\n");

   print("<TR>\n");
   for ($c=0; $c<count($this->columns); $c++)
     if ($this->columns[$c]->display){
       print("<TD><FONT FACE=\"Arial,Helvetica\">".$this->columns[$c]->title."</FONT></TD>\n");
       $numCols++;
     }
   print("</TR>\n");

   print("<TR>\n");
   for ($c=0; $c<count($this->columns); $c++)
     if ($this->columns[$c]->display){
       print("<TD>\n");
       if ($rec)
	 $val = (($rec[$this->columns[$c]->name])?$rec[$this->columns[$c]->name]:"");
       else if ($defaultValues)
	 $val = (($defaultValues[$this->columns[$c]->formName])?$defaultValues[$this->columns[$c]->formName]:"");
       else
	 $val = "";
       $this->writeFormElement($this->columns[$c],$val);
       print("</TD>\n");
     }
   print("</TR>\n");

   print("<TR><TD COLSPAN=".($numCols)." ALIGN=RIGHT>\n");
   print($submitCode?$submitCode:" <INPUT TYPE=SUBMIT VALUE=\" Update \">\n");
   print("</TD></TR>\n");

   print("</TABLE>\n");
   print("</FORM>\n");

   return ((!$row)||$rec);
  }// writeUpdateForm


  // Called by writeUpdateForm() to write out hidden form elements.
  // This would usually include those columns not marked for display and
  // a dump of the request object to maintain state.
  function writeHiddenFormValues($rec)
  {
   if ($rec){
     for ($c=0; $c<count($this->columns); $c++)
       if (!$this->columns[$c]->display)
	 print(" <INPUT TYPE=HIDDEN NAME=\"".$this->columns[$c]->formName."\" VALUE=\"".$rec[$this->columns[$c]->name]."\">\n");
   }

   // for ( item in request )
   //   print(" <INPUT TYPE=HIDDEN NAME=".item." VALUE=\"".request[item]."\" >\n");

   return (true);
  }// writeHiddenFormValues


  // Called by writeUpdateForm() to write out individual form elements.
  // Override this if you want to change from the default text field - e.g.
  // to display select lists or check boxes.
  function writeFormElement($col,$val)
  {

   print("<INPUT TYPE=TEXT NAME=\"".$col->formName."\" SIZE=\"".$col->length."\" VALUE=\"".$val."\"></TD>\n");

   return (true);
  }// writeFormElement
  
 }
 
?>
