<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Prototype class for use with the emp_ident table.
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


 class EmpIdent
 {
  var $tableName = 'emp_ident';
  
  function EmpIdent()
  {
  }


  // Returns a cursor for the rows where the name column matches the
  // specified name.  It first trys a "NAME LIKE 'firstName%surname'" and
  // if this doesn't return anything a "NAME LIKE '%firstName%surname%'".
  // e.g. call - name2recList("Joe Smith");
  function name2recList(&$db,$name)
  {
   $name = preg_replace("/[' ]/g","%",strtoupper($name));
   $selectStmt = "SELECT cid,core_id,name FROM ".$this->tableName." WHERE UPPER(name) LIKE ";

   $db->query(selectStmt."'".$name."' ORDER BY name");
   if (!$db->next_record())
     $db->query(selectStmt."'%".$name."%' ORDER BY name");

   return ($db);
  }// name2recList


  // Function that returns an object with some employee data for a CID.
  // Returns null of no match found. e.g. call - cid2record("12345678");
  function cid2record(&$db,$cid)
  {
   $db->query("SELECT e.cid,e.name,e.core_id,NVL(s.name,'('||e.scid||')') AS manager FROM ".$this->tableName." e, ".$this->tableName." s WHERE e.scid = s.cid(+) AND e.cid ='".$cid."'");

   if (!$db->next_record())
     return (0);
   else
     return ($db->Record);
  }// cid2record


  // Returns a cursor for the rows that match the whereClause sorted on
  // the specified column. e.g. call - getRows("name LIKE 'Paul%',"cid");
  function getRows(&$db,$whereClause,$order)
  {
   return ($db->query("SELECT cid,core_id,name FROM ".$this->tableName.($whereClause?(" WHERE ".$whereClause):"").($order?(" ORDER BY ".$order):"")));
  }// getRows


  // Writes out a form for selecting an employee by CID.  The currently
  // selected employee (determined from the argument or from a cookie)
  // is identified with their name & supervisor. e.g. call - 
  // writeSelectForm("12345678");
  function writeSelectForm($cid)
  {
   global $HTTP_COOKIE_VARS;
   $cid = $cid?$cid:$HTTP_COOKIE_VARS[$this->tableName."_cid"];

   // print("<SCRIPT LANGUAGE=\"JavaScript\">\n");
   // print(" <!--\n\n");
   // print(" ".looksLikeCID.toString()."\n");
   // print(" ".validateCIDForm.toString()."\n");
   // print("\n // -->\n");
   // print("</SCRIPT>\n");

   print("<FORM METHOD=POST onSubmit=\"return validateCIDForm(this);\">\n");
   print("<TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
   print(" <TABLE BORDER=0 WIDTH=100%><TR>\n  <TD ALIGN=LEFT>&nbsp;</TD>\n");
   print("  <TD ALIGN=LEFT><FONT FACE=\"Arial,Helvetica\">\n");

   if ($cid){
     $emp = $this->cid2record($db,$cid);
     if ( $emp )
       print("Employee: ".$emp['name']."<BR>Supervisor: ".$emp['manager']."</TD>\n");
     else
       print($cid." - Unknown (or ex) employee.</FONT></TD>\n");
     setcookie($this->tableName."_cid",$cid);
   }else{
     print("Please select an employee:");
   }

   print("  </FONT></TD>\n  <TD ALIGN=RIGHT><CENTER>\n");
   print("   <INPUT TYPE=TEXT SIZE=8 NAME=\"cid\" VALUE=\"".($cid?$cid:"")."\">\n");
   print("   <BR><A HREF=\"javascript:null;\" onClick=\"window.open('empSearch.html?submitTarget=true','empSearch','width=250,height=150,resizable=1,scrollbars=yes'); return false;\">".($cid?"Select New":"Search by Name")."</A>\n");
   print("  </CENTER></TD>\n </TR></TABLE>\n</TD></TR></TABLE>\n");
   print("</FORM>\n");

   return ($cid);
  }// writeSelectForm


  // Writes out a table listing employee names with a field as a link
  // to select that employee.  The specified search field is usually CID,
  // or core ID and defaults to CID.  A target form & field can also
  // be specified.  If request.name exists all employees matching that
  // name are listed.  If request.initial exists all records matching
  // that where clause are listed.  If neither are given the method
  // does nothing & returns null.
  function writeList(&$form)
  {
   $field = $form['searchField']?$form['searchField']:"cid";

   if ( $form['name'] )
     $list = $this->name2recList($form['name']);
   else if ( $form['initial'] )
     $list = $this->getRows($form['initial'],"name");

   if (!$list) return (false);

   if ( count($list)>0 ){
     print("<TABLE BORDER=0>\n");
     while ($list->next_record()){
       if ( $list[strtoupper($field)] )
	 print("<TR><TD><A HREF=\"javascript:window.close();\" onClick=\"window.opener.document.forms['".($form['targetForm']?$form['targetForm']:"0")."']['".($form['targetField']?$form['targetField']:$field)."'].value='".$list[strtoupper($field)]."';".(($form['submitTarget']=="true")?("window.opener.document.forms['".($form['targetForm']?$form['targetForm']:"0")."'].submit();"):(""))."\">".$list[strtoupper($field)]."</A></TD><TD>".$list['NAME']."</TD></TR>\n");
       else
	 print("<TR><TD>-</TD><TD>".$list['NAME']."</TD></TR>\n");
     }
     print("</TABLE>\n");
   }else{
     print("No match\n");
   }

   return (true);
  }// writeList
  
 }
 
?>
