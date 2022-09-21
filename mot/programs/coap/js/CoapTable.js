
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990909
//  TestedWith:		ES 362
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        Prototype object for COAP tables.
//  Keywords:            
//  Notes:		

function CoapTable(name)
{
 // extends StandardTable
 this.parent = StandardTable;
 this.parent(name,project.tableDef[name]);

 // properties
 this.clientFuncs.push("looksLikeDate");
 this.clientFuncs.push("looksLikeCID");
 this.updateFormParams = "METHOD=POST ACTION=\"main.html\" TARGET=\"coap_main\"";
 this.updateFormSubmitCode = " <INPUT TYPE=BUTTON VALUE=\" Cancel \" onClick=\"window.close();\">\n <INPUT TYPE=BUTTON VALUE=\" Update \" onClick=\"if ( validateUpdateForm(this.form) ) { this.form.submit(); window.close(); }\">\n";

 // methods
 this.validDataAuxCheck = CoapTable_validDataAuxCheck;
 this.writeAuxClientCheck = CoapTable_writeAuxClientCheck;
 this.augmentRequest = CoapTable_augmentRequest;
 this.writeFormElement = CoapTable_writeFormElement;
 this.editLinkStr = CoapTable_editLinkStr;
 this.writeHiddenFormValues = CoapTable_writeHiddenFormValues;

}// CoapTable


// Adds additional server side checks for the date and CID fields in
// the COAP tables.  Returns false if the date fields are not valid
// dates or if the CIDs do not match /[0-9]{8}/
function CoapTable_validDataAuxCheck(request)
{
 for (var c=0; c<this.columns.length; c++){
   var col = this.columns[c];
   if ( col.type=="date" && request[col.formName] && !request[col.formName].toString().match(/00:00:00/)){
     this.errorMessage = "The "+col.title+" field should be of the form DD-MON-YYYY.";
     return (false);
   }
   if ( col.name.match(/cid$/) && request[col.formName] && !looksLikeCID(request[col.formName]) ){
     this.errorMessage = "The "+col.title+" field is not a valid CID.";
     return (false);
   }
 }

 return (true);
}// CoapTable_validDataAuxCheck


// Adds additional client side checks for the date and CID fields in
// the COAP tables.  Returns false if the date fields are not of the form
// DD-MON-YYYY or if the CIDs do not match /[0-9]{8}/
function CoapTable_writeAuxClientCheck()
{
 for (var c=0; c<this.columns.length; c++){
   var col = this.columns[c];
   if ( col.type=="date" ){
     write("  if ("+(col.canBeNull?"form."+col.formName+".value!=\"\" && ":"")+"!looksLikeDate(form."+col.formName+".value)){\n");
     write("    alert(\"The "+col.title+" field should be of the form DD-MON-YYYY!\");\n");
     write("    return (false);\n");
     write("  }\n\n");
   }
   if ( col.name.match(/cid$/) ){
     write("  if ("+(col.canBeNull?"form."+col.formName+".value!=\"\" && ":"")+"!looksLikeCID(form."+col.formName+".value)){\n");
     write("    alert(\"The "+col.title+" field is not a valid CID.\");\n");
     write("    return (false);\n");
     write("  }\n\n");
   }
 }

 return (true);
}// CoapTable_writeAuxClientCheck


// Updates the request object with additional properties - converts
// each date string to a date object and adds rec number from the
// respective sequence if no rec number is given.  Returns the
// modified request object.
function CoapTable_augmentRequest(request)
{
 if (this.requestAugmented) return (request);

 for (var c=0; c<this.columns.length; c++){
   var col = this.columns[c];
   if ( col.type=="date" ){
     if (request[col.formName]=="") request[col.formName] = null;
     if (request[col.formName] && request[col.formName].match(/^[0-9]{2}-[A-Za-z]{3}-[0-9]{4}$/)){
       var dateElements = request[col.formName].split("-");
       request[col.formName] = new Date(dateElements[1]+", "+dateElements[0]+" "+dateElements[2]);
     }
   }
 }

 if (!request.rec || isNaN(request.rec)){
   key = database.superCursor("SELECT "+this.tableName+"_rec.nextVal AS next_rec FROM dual");
   key.next();
   request.rec = key.NEXT_REC;
   key.close();
 }

 this.requestAugmented = true;

 return (request);
}// CoapTable_augmentRequest


// Overrides standard sub-method used by writeUpdateForm() to generate
// each form element. Writes a pick-list for each of the list fields
// and munges the default dates to DD-MON-YYYY.
function CoapTable_writeFormElement(col,val)
{
 var colName = col.name.toUpperCase();

 if (col.type=="list"){
   var pickList = new PickList(project.col2table[this.tableName+"."+col.name]);
   pickList.writeSelectList(col.formName,val);
 }else if (col.type=="date"){
   write("<INPUT TYPE=TEXT NAME=\""+col.formName+"\" SIZE=\""+col.length+"\" VALUE=\""+col.munge(val)+"\">\n");
 }else{
   write("<INPUT TYPE=TEXT NAME=\""+col.formName+"\" SIZE=\""+col.length+"\" VALUE=\""+val+"\">\n");
 }

 return (true);
}// CoapTable_writeFormElement


// Overrides standard sub-method used by writeUpdateForm() to write
// hidden form values - adds a dump of the request object in order
// to maintain state - in particular the rec field.
function CoapTable_writeHiddenFormValues(rec)
{
 if (rec){
   for (var c=0; c<this.columns.length; c++)
     if (!this.columns[c].display)
       write(" <INPUT TYPE=HIDDEN NAME=\""+this.columns[c].formName+"\" VALUE=\""+rec[this.columns[c].name.toUpperCase()]+"\">\n");
 }

 for ( item in request )
   write(" <INPUT TYPE=HIDDEN NAME="+item+" VALUE=\""+request[item]+"\" >\n");

 return (true);
}// CoapTable_writeHiddenFormValues


// Overrides standard sub-method used by writeTable() to generate the
// link for updating a record.  Writes a link to open a new window with
// an update form.
function CoapTable_editLinkStr(row)
{
 return ("<A HREF=\"javascript:null;\" onClick=\"window.open('update.html?tab="+request.tab+"&row="+row.REC+"','coap_update','width=500,height=140,resizable=1,scrollbars=yes'); return false;\"><IMG SRC=\"images/pen.gif\" BORDER=0 WIDTH=34 HEIGHT=19></A>");
}// CoapTable_editLinkStr


