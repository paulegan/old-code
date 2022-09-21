
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990909
//  TestedWith:		ES 362
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        Pick-list object that slightly modifies a standard table.
//  Keywords:            
//  Notes:		Assumes specified table has a column called "name"


function PickList(name)
{
 // extends StandardTable
 this.parent = StandardTable;
 this.parent(name,project.tableDef[name]);

 // Methods
 this.writeSelectList = PickList_writeSelectList;
 this.writeUpdateForm = PickList_writeUpdateForm;

}// PickList


// Writes a single or multiple select list called "formName" with 
// the option matching "sel" selected. e.g. call - writeSelectList("name","",true);
function PickList_writeSelectList(formName,sel,multi)
{
 var list = database.superCursor("SELECT name FROM "+this.tableName);

 write("<SELECT NAME=\""+formName+"\""+(multi?("MULTIPLE SIZE="+((list.length<30)?list.length:30)):"")+">\n");

 while (list.next())
   write("  <OPTION"+((list.NAME==sel)?" SELECTED":"")+"> "+list.NAME+"\n");
 list.close();

 write("</SELECT>\n");

 return (true);
}// PickList_writeSelectList


// Overrides the standard writeUpdateForm() in favour of a simpler
// one with one text field and a submit button.
function PickList_writeUpdateForm()
{
 this.writeClientCheck("validateUpdateForm");

 write("  <FORM METHOD=POST onSubmit=\"return validateUpdateForm(this);\">\n");
 write("  <INPUT TYPE=HIDDEN NAME=\"table\" VALUE=\""+request.table+"\">\n");
 write("  <INPUT TYPE=HIDDEN NAME=\"add\" VALUE=\"true\">\n");
 write("  <TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
 write("   <TABLE WIDTH=100% BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>\n");
 write("     <TD ALIGN=LEFT><INPUT TYPE=TEXT SIZE=20 NAME=\"name\"></TD>\n");
 write("     <TD ALIGN=RIGHT><INPUT TYPE=SUBMIT VALUE=\" Add New Entry \"></TD>\n");
 write("   </TR></TABLE>\n");
 write("  </TD></TR></TABLE>\n");
 write("  </FORM>\n");

 return (true);
}// PickList_writeUpdateForm
