
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971208
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        Prototype object for use with the emp_ident table.
//  Keywords:            
//  Notes:		


function EmpIdent()
{

 // Properties
 this.tableName = "emp_ident";

 // Methods
 this.name2recList = EmpIdent_name2recList;
 this.cid2record = EmpIdent_cid2record;
 this.getRows = EmpIdent_getRows;
 this.writeSelectForm = EmpIdent_writeSelectForm;
 this.writeList = EmpIdent_writeList;

}// EmpIdent


// Returns a cursor for the rows where the name column matches the
// specified name.  It first trys a "NAME LIKE 'firstName%surname'" and
// if this doesn't return anything a "NAME LIKE '%firstName%surname%'".
// e.g. call - name2recList("Joe Smith");
function EmpIdent_name2recList(name)
{
 var cursor;
 var name = name.toUpperCase().replace(/[' ]/g,"%");
 var selectStmt = "SELECT cid,core_id,name FROM "+this.tableName+" WHERE UPPER(name) LIKE ";

 cursor = database.superCursor(selectStmt+"'"+name+"' ORDER BY name");
 if (cursor.length<1){
   cursor.close();
   cursor = database.superCursor(selectStmt+"'%"+name+"%' ORDER BY name");
 }

 return (cursor);
}// EmpIdent_name2recList


// Function that returns an object with some employee data for a CID.
// Returns null of no match found. e.g. call - cid2record("12345678");
function EmpIdent_cid2record(cid)
{
 var temp = database.superCursor("SELECT e.cid,e.name,e.core_id,NVL(s.name,'('||e.scid||')') AS manager FROM "+this.tableName+" e, "+this.tableName+" s WHERE e.scid = s.cid(+) AND e.cid ='"+cid+"'");

 if (!temp.next()){
    temp.close();
    return (null);
 }

 return (temp);
}// EmpIdent_cid2record


// Returns a cursor for the rows that match the whereClause sorted on
// the specified column. e.g. call - getRows("name LIKE 'Paul%',"cid");
function EmpIdent_getRows(whereClause,order)
{
 return (database.superCursor("SELECT cid,core_id,name FROM "+this.tableName+(whereClause?(" WHERE "+whereClause):"")+(order?(" ORDER BY "+order):"")));
}// EmpIdent_getRows


// Writes out a form for selecting an employee by CID.  The currently
// selected employee (determined from the argument or from a cookie)
// is identified with their name & supervisor. e.g. call - 
// writeSelectForm("12345678");
function EmpIdent_writeSelectForm(cid)
{
 cid = cid?cid:client[this.tableName+"_cid"];

 write("<SCRIPT LANGUAGE=\"JavaScript\">\n");
 write(" <!--\n\n");
 write(" "+looksLikeCID.toString()+"\n");
 write(" "+validateCIDForm.toString()+"\n");
 write("\n // -->\n");
 write("</SCRIPT>\n");
 
 write("<FORM METHOD=POST onSubmit=\"return validateCIDForm(this);\">\n");
 write("<TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
 write(" <TABLE BORDER=0 WIDTH=100%><TR>\n  <TD ALIGN=LEFT>&nbsp;</TD>\n");
 write("  <TD ALIGN=LEFT><FONT FACE=\"Arial,Helvetica\">\n");

 if (cid){
   var emp = this.cid2record(cid);
   if ( emp ){
     write("Employee: "+emp.NAME+"<BR>Supervisor: "+emp.MANAGER+"</TD>\n");
     emp.close();
   }else{
     write(cid+" - Unknown (or ex) employee.</FONT></TD>\n");
   }
   client[this.tableName+"_cid"] = cid;
 }else{
   write("Please select an employee:");
 }

 write("  </FONT></TD>\n  <TD ALIGN=RIGHT><CENTER>\n");
 write("   <INPUT TYPE=TEXT SIZE=8 NAME=\"cid\" VALUE=\""+(cid?cid:"")+"\">\n");
 write("   <BR><A HREF=\"javascript:null;\" onClick=\"window.open('empSearch.html?submitTarget=true','empSearch','width=250,height=150,resizable=1,scrollbars=yes'); return false;\">"+(cid?"Select New":"Search by Name")+"</A>\n");
 write("  </CENTER></TD>\n </TR></TABLE>\n</TD></TR></TABLE>\n");
 write("</FORM>\n");

 return (cid);
}// EmpIdent_writeSelectForm


// Writes out a table listing employee names with a field as a link
// to select that employee.  The specified search field is usually CID,
// or core ID and defaults to CID.  A target form & field can also
// be specified.  If request.name exists all employees matching that
// name are listed.  If request.initial exists all records matching
// that where clause are listed.  If neither are given the method
// does nothing & returns null.
function EmpIdent_writeList(request)
{
 var list = null;
 var field = request.searchField?request.searchField:"cid";

 if ( request.name )
   list = empIdent.name2recList(request.name);
 else if ( request.initial )
   list = empIdent.getRows(request.initial,"name");

 if (!list) return (false);

 if ( list.length>0 ){
   write("<TABLE BORDER=0>\n");
   while (list.next()){
     if ( list[field.toUpperCase()] )
       write("<TR><TD><A HREF=\"javascript:window.close();\" onClick=\"window.opener.document.forms['"+(request.targetForm?request.targetForm:"0")+"']['"+(request.targetField?request.targetField:field)+"'].value='"+list[field.toUpperCase()]+"';"+((request.submitTarget=="true")?("window.opener.document.forms['"+(request.targetForm?request.targetForm:"0")+"'].submit();"):(""))+"\">"+list[field.toUpperCase()]+"</A></TD><TD>"+list.NAME+"</TD></TR>\n");
     else
       write("<TR><TD>-</TD><TD>"+list.NAME+"</TD></TR>\n");
   }
   write("</TABLE>\n");
 }else{
   write("No match\n");
 }
 list.close();

 return (true);
}// EmpIdent_writeList
