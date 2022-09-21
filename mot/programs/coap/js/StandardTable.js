
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990908
//  TestedWith:		ES 362 & Oracle 806
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        Prototype object for a standard table. Contains methods for operations on this table.
//  Keywords:            
//  Notes:		



function StandardTable(tableName,columns,key)
{

 // Properties
 this.tableName = tableName || "dual";
 this.columns = columns || [ new ColumnSpec() ];
 this.key = key || this.columns[0].name;
 this.requestAugmented = false;
 this.errorMessage = "";
 this.clientFuncs = [];

 for (var c=0; c<this.columns.length; c++)
   if (this.columns[c].name==this.key)
     this.keyFormName = this.columns[c].formName;

 // Methods
 this.getRows = StandardTable_getRows;
 this.update = StandardTable_update;
 this.augmentRequest = StandardTable_augmentRequest;
 this.validData = StandardTable_validData;
 this.validDataAuxCheck = StandardTable_validDataAuxCheck;
 this.writeClientCheck = StandardTable_writeClientCheck;
 this.writeAuxClientCheck = StandardTable_writeAuxClientCheck;
 this.writeTable = StandardTable_writeTable;
 this.editLinkStr = StandardTable_editLinkStr;
 this.writeUpdateForm = StandardTable_writeUpdateForm;
 this.writeHiddenFormValues = StandardTable_writeHiddenFormValues;
 this.writeFormElement = StandardTable_writeFormElement;

}// StandardTable


// Returns a cursor for the specified rows in the specified order.
function StandardTable_getRows(whereClause,order)
{
 return (database.superCursor("SELECT * FROM "+this.tableName+(whereClause?(" WHERE "+whereClause):"")+(order?(" ORDER BY "+order):"")));
}// StandardTable_getRows


// Function to update one row (select by key) in the table with the
// values given in the request object. If a row with the specified key
// does not exist then a new row is created.  Will return false and set
// errorMessage if anything goes wrong.
function StandardTable_update(request)
{
 var record;
 var upd=false;
 var result;

 if (!this.validData(request))
   return (false);

 database.beginTransaction();
 record = database.superCursor("SELECT * FROM "+this.tableName+" WHERE "+this.key+" = '"+request[this.keyFormName]+"'",true);

 if (record.next())
   upd=true;

 record.assign(request,this.columns);

for ( prop in record )
  debug("stu: "+prop+"="+record[prop]);

 if (upd)
   result = record.updateRow(this.tableName);
 else
   result = record.insertRow(this.tableName);

 if (result == 0){
   database.commitTransaction();
   record.close();
   return (true);
 }else{
   this.errorMessage = "Transaction rolled back! "+database.majorErrorMessage();
   database.rollbackTransaction();
   record.close();
   return (false);
 }
 
}// StandardTable_update


// Override this method if there are extra properties to be
// added to the request object or properties are to be modified.
function StandardTable_augmentRequest(request)
{
 if (this.requestAugmented) return (request);

 // Add any additional properties to the request object here. e.g. - 
 // request[key] = project.Seq++;
 // request.name = request.name.toUpperCase();

 this.requestAugmented = true;

 return (request);
}// StandardTable_augmentRequest


// Writes out a function that should be used in a form onSubmit
// tag.  Returns false if any form element that has the validateOnClient
// flag set and does not match the contraints in the columns array.
// Use the clientFuncs array and the writeAuxClientCheck() method to
// extend the functionality.
function StandardTable_writeClientCheck(funcName)
{
 var cols = this.columns;

 write("<SCRIPT LANGUAGE=\"JavaScript\">\n");
 write(" <!--\n\n");

 for (var f=0; f<this.clientFuncs.length; f++)
   write(" "+eval(this.clientFuncs[f]+".toString()")+"\n");

 write(" function "+(funcName?funcName:"validateUpdateForm")+"(form)\n");
 write(" {\n");

 for (var c=0; c<cols.length; c++){
   if (cols[c].validateOnClient){
     if (!cols[c].canBeNull){
       write("  if (form."+cols[c].formName+".value==\"\"){\n");
       write("    alert (\"Please fill in the "+cols[c].title+" field.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
     if (cols[c].type=="text"){
       write("  if (form."+cols[c].formName+".value.length>"+cols[c].length+"){\n");
       write("    alert (\"The "+cols[c].title+" field should be "+cols[c].length+" characters or less.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
     if (cols[c].type=="number"){
       write("  if (isNaN(form."+cols[c].formName+".value)){\n");
       write("    alert (\"The "+cols[c].title+" field does not contain a valid number.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
   }
 }

 this.writeAuxClientCheck();

 write("  return (true);\n");
 write(" }\n");

 write("\n // -->\n");
 write("</SCRIPT>\n");

 return (true);
}// StandardTable_writeClientCheck


// Use to add extra client side checks.
function StandardTable_writeAuxClientCheck()
{
 // e.g. - 
 // write("  if (!form.name.value.match(/^[A-Z]*$/)){\n");
 // write("    alert (\"The name must contain only capital letters.\");\n");
 // write("    return (false);\n");
 // write("  }\n");

 return (true);
}// StandardTable_writeAuxClientCheck


// Returns false if any fields on the request object to not match
// the constraints specified in the columns array.  The request object
// is first augmented with the augmentRequest method.  Extend the
// checks with the validDataAuxCheck() method.
function StandardTable_validData(request)
{
 var request = this.augmentRequest(request);
 var cols = this.columns;

 for (var c=0; c<cols.length; c++){
   var value = (request[cols[c].formName]!="")?request[cols[c].formName]:null;
   if ( !cols[c].canBeNull && !value ){
     this.errorMessage = "Mandatory field, "+cols[c].title+", not entered.";
     return (false);
   }
   if ( cols[c].type=="text" && value && value.length>cols[c].length ){
     this.errorMessage = cols[c].title+" should be "+cols[c].length+" characters or less.";
     return (false);
   }
   if ( cols[c].type=="number" && value && isNaN(value) ){
     this.errorMessage = cols[c].title+" is not a valid number!";
     return (false);
   }
 }

 if (!this.validDataAuxCheck(request))
   return (false);

 return (true);
}// StandardTable_validData


// Used to add extra server side checks on the request properties.
// Set this.errorMessage if returning false.
function StandardTable_validDataAuxCheck(request)
{
 // e.g. - 
 // if (!request.name.match(/^[A-Z]*$/){
 //   this.errorMessage = "The name must contain only capital letters.";
 //   return (false);
 // }

 return (true);
}// StandardTable_validDataAuxCheck


// Writes out an HTML table containing all the rows returned by
// the whereClause and in the specified order.  Only columns whose
// display flag is set are shown and values are run through the munge
// function if it was given.  If incEditColumn is set then an
// edit column is added, the contents of which is given by the
// editLinkStr() method.
function StandardTable_writeTable(whereClause,order,incEditColumn)
{
 var row = this.getRows(whereClause,order);

 write("<TABLE WIDTH=90% BORDER=1 CELLPADDING=2 CELLSPACING=0>\n");

 if (row.length>0){

   write("<TR BGCOLOR=#999999>\n");
   for (var c=0; c<this.columns.length; c++)
     if (this.columns[c].display)
       write("<TD><FONT FACE=\"Arial,Helvetica\">"+this.columns[c].title+"</FONT></TD>\n");
   if (incEditColumn)
     write("<TD><FONT FACE=\"Arial,Helvetica\">Edit</FONT></TD>\n");
   write("</TR>\n");

   while (row.next()){
     write("<TR BGCOLOR=#FFFFFF>\n");
     for (var c=0; c<this.columns.length; c++){
       var colName = this.columns[c].name.toUpperCase();
       if (this.columns[c].display)
 	 if (this.columns[c].munge)
 	   write("<TD>"+((row[colName])?this.columns[c].munge(row[colName]):"&nbsp;")+"</TD>");
 	 else
 	   write("<TD>"+((row[colName])?row[colName]:"&nbsp;")+"</TD>");
     }
     if (incEditColumn) write("<TD>"+this.editLinkStr(row)+"</TD>\n");
     write("</TR>\n");
   }

 }else{
   write("<TR><TD ALIGN=CENTER>No records found.</TD></TR>\n");
 }

 write("</TABLE>\n");
 row.close();

 return (true);
}// StandardTable_writeTable


// This method is used to determine what should be printed in the edit
// column of the table from writeTable().  Below is an example but this
// method should almost certainly be overridden.
function StandardTable_editLinkStr(row)
{
 return ("<A HREF=\""+request.uri+"?table="+this.tableName+"&"+this.keyFormName+"="+row[this.key.toUpperCase()]+"\">EDIT</A>");
}// StandardTable_editLinkStr


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
function StandardTable_writeUpdateForm(row,defaultValues,formParams,submitCode,isUpdateFlag)
{
 var rec = null;
 var numCols = 0;
 var val;

 if (row){
   rec = this.getRows(this.key+" = '"+row+"'");
   if (!rec.next())
     rec = null;
 }

 this.writeClientCheck();

 write("<FORM "+(formParams?formParams:"METHOD=POST onSubmit=\"return validateUpdateForm(this);\"")+">\n");
 if (isUpdateFlag) write(" <INPUT TYPE=HIDDEN NAME="+isUpdateFlag+" VALUE=\"true\" >\n");

 this.writeHiddenFormValues(rec);

 write("<TABLE WIDTH=90% BORDER=0 CELLPADDING=2 CELLSPACING=0>\n");

 write("<TR>\n");
 for (var c=0; c<this.columns.length; c++)
   if (this.columns[c].display){
     write("<TD><FONT FACE=\"Arial,Helvetica\">"+this.columns[c].title+"</FONT></TD>\n");
     numCols++;
   }
 write("</TR>\n");

 write("<TR>\n");
 for (var c=0; c<this.columns.length; c++)
   if (this.columns[c].display){
     write("<TD>\n");
     if (rec)
       val = ((rec[this.columns[c].name.toUpperCase()])?rec[this.columns[c].name.toUpperCase()]:"");
     else if (defaultValues!=null)
       val = ((defaultValues[this.columns[c].formName])?defaultValues[this.columns[c].formName]:"");
     else
       val = "";
     this.writeFormElement(this.columns[c],val);
     write("</TD>\n");
   }
 write("</TR>\n");

 if (rec) rec.close();

 write("<TR><TD COLSPAN="+(numCols)+" ALIGN=RIGHT>\n");
 write(submitCode?submitCode:" <INPUT TYPE=SUBMIT VALUE=\" Update \">\n");
 write("</TD></TR>\n");

 write("</TABLE>\n");
 write("</FORM>\n");

 return ((!row)||rec);
}// StandardTable_writeUpdateForm


// Called by writeUpdateForm() to write out hidden form elements.
// This would usually include those columns not marked for display and
// a dump of the request object to maintain state.
function StandardTable_writeHiddenFormValues(rec)
{
 if (rec){
   for (var c=0; c<this.columns.length; c++)
     if (!this.columns[c].display)
       write(" <INPUT TYPE=HIDDEN NAME=\""+this.columns[c].formName+"\" VALUE=\""+rec[this.columns[c].name.toUpperCase()]+"\">\n");
 }

 // for ( item in request )
 //   write(" <INPUT TYPE=HIDDEN NAME="+item+" VALUE=\""+request[item]+"\" >\n");

 return (true);
}// StandardTable_writeHiddenFormValues


// Called by writeUpdateForm() to write out individual form elements.
// Override this if you want to change from the default text field - e.g.
// to display select lists or check boxes.
function StandardTable_writeFormElement(col,val)
{

 write("<INPUT TYPE=TEXT NAME=\""+col.formName+"\" SIZE=\""+col.length+"\" VALUE=\""+val+"\"></TD>\n");

 return (true);
}// StandardTable_writeFormElement

