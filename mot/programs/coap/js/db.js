
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971026
//  TestedWith:		ES 3.51,3.61,3.62 & Oracle 7.3.2.1,8.0.4.3
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            19981028,paul,Added cursor wrapper.
//  History:            19990623,paul,Added cursor assign.
//  Description:        Adds extra functionality to database object by adding new methods & a new datatype.  Main features include more robust connectivity & and the ArrayCursor object which doesn't maintain an open connection.
//  Keywords:            
//  Notes:		Modifies the database object for ALL apps. Requires an error(msg) function.



// Data type for specifying table column properties.
function ColumnSpec(name,formName,canBeNull,type,length,validateOnClient,title,display,munge)
{
 this.name = name || "dummy";
 this.formName = formName || "dummy";
 this.canBeNull = canBeNull;
 this.type = type || "text";
 this.length = length || 1;
 this.validateOnClient = validateOnClient;
 this.title = title || "dummy";
 this.display = display;
 this.munge = munge;
}// ColumnSpec


// String method for ensuring no 8 bit characters are sent to the
// database.  Replaces such characters with "project.extCharReplacement".
function String_stripToASCII7()
{
  var text = this.toString();
  var laundry = "";
  for (var c=0; c<text.length; c++)
    laundry += ((text.charCodeAt(c)>0) && (text.charCodeAt(c)<128))?text.charAt(c):project.extCharReplacement;
  return (laundry);
}

// Constructor for ArrayCursor object.  Copies the data
// from a cursor and stores it in an array.  The main data
// structure is an array of rows, where each row has a
// property for each column.  Note that the column properties
// of this object are case sensitive - unlike a cursor.
function ArrayCursor(cursor)
{
 // Properties
 this.length = 0;
 this.rows = new Array();
 this.columnNames = new Array();
 this.errorMessage = "";

 // Methods
 this.next = ArrayCursor_next;
 this.close = ArrayCursor_close;
 this.columns = ArrayCursor_columns;
 this.columnName = ArrayCursor_columnName;
 this.validData = ArrayCursor_validData;
 this.writeClientCheck = ArrayCursor_writeClientCheck;
 this.assign = ArrayCursor_assign;
 this.deleteRow = ArrayCursor_null;
 this.insertRow = ArrayCursor_null;
 this.updateRow = ArrayCursor_null;

 // Initialise some props for a true cursor
 if (cursor){
   for (var i=0; i<cursor.columns(); i++){
     this.columnNames.push(cursor.columnName(i));
     this[cursor.columnName(i)] = "";
   }
   while (cursor.next()){
     var row = new Array();
     for (var i=0; i<this.columnNames.length; i++)
       row[this.columnNames[i]] = cursor[this.columnNames[i]];
     this.rows.push(row);
     this.length++;
   }
 }
}// ArrayCursor


// Sets the "column properties" of the cursor object to the
// next in the array.  The head of the array is removed.
// Returns false if there's nothing left.
function ArrayCursor_next()
{
 var row = this.rows.shift();

 if (row==null) return (false);

 for (var i=0; i<this.columnNames.length; i++)
   this[this.columnNames[i]] = row[this.columnNames[i]];

 return (true);
}// ArrayCursor_next


// Truncates the internal arrays.
function ArrayCursor_close()
{
 this.length = 0;
 for (var i=0; i<this.rows.length; i++)
   this.rows[i].length = 0;
 this.rows.length = 0;
 this.columnNames.length = 0;
}// ArrayCursor_close


// Returns the number of columns.
function ArrayCursor_columns()
{
 return (this.columnNames.length);
}// ArrayCursor_columns


// Returns the ith column name.
function ArrayCursor_columnName(i)
{
 return (this.columnNames[i]);
}// ArrayCursor_columnName


// Returns true or false depending on whether the request data
// breaks any constraints.  Sets errorMessage if false.
function ArrayCursor_validData(request, cols)
{
 for (var c=0; c<cols.length; c++){
   var value = (request[cols[c].formName]!="")?request[cols[c].formName]:null;
   if ( !cols[c].canBeNull && !value ){
     this.errorMessage = "Mandatory field, "+cols[c].formName+", not entered.";
     return (false);
   }
   if ( cols[c].type=="text" && value && value.length>cols[c].length ){
     this.errorMessage = cols[c].formName+" should be "+cols[c].length+" characters or less.";
     return (false);
   }
   if ( cols[c].type=="number" && value && isNaN(value) ){
     this.errorMessage = cols[c].formName+" is not a valid number!";
     return (false);
   }
 }

 return (true);
}// ArrayCursor_validData


// Writes out the code for a function that will check the
// values of a form.  The client function will return false
// is any form element fails any column constraint.  To be
// used with onSubmit().
function ArrayCursor_writeClientCheck(cols)
{
 write(" function checkFormValues(form)\n {\n\n");
 for (var c=0; c<cols.length; c++){
   if (cols[c].validateOnClient){
     if (!cols[c].canBeNull){
       write("  if (form."+cols[c].formName+".value==\"\"){\n");
       write("    alert (\"Please fill in the "+cols[c].formName+" field.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
     if (cols[c].type=="text"){
       write("  if (form."+cols[c].formName+".value.length>"+cols[c].length+"){\n");
       write("    alert (\"The "+cols[c].formName+" field should be "+cols[c].length+" characters or less.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
     if (cols[c].type=="number"){
       write("  if (isNaN(form."+cols[c].formName+".value)){\n");
       write("    alert (\"The "+cols[c].formName+" field does not contain a valid number.\");\n");
       write("    return (false);\n");
       write("  }\n");
     }
   }
 }
 write("  return (true);\n");
 write(" }\n");

 return (true);
}// ArrayCursor_writeClientCheck


// Takes the columns specified and assigns the values from
// the corresponding request properties.
function ArrayCursor_assign(request, cols)
{
 var col;

 for (var c=0; c<cols.length; c++){
   col = cols[c].name.toUpperCase();
   switch (cols[c].type) {
    case "datelist" : 
      this[col] = (new DateLists(cols[c].formName)).selected();
      break; 
    case "list" : 
      this[col] = getOptionValue(cols[c].formName,0);
      break;
    case "text" : 
      this[col] = (request[cols[c].formName])?(request[cols[c].formName].stripToASCII7()):(null);
      break;
    default : 
      this[col] = request[cols[c].formName]?request[cols[c].formName]:null;
   } 
   if (this[col]=="" || this[col]=="null" || this[col]=="undefined")
     this[col] = null;
 }
 return (true);
}// ArrayCursor_assign


// Does nothing - for unimplemented methods.
function ArrayCursor_null()
{
 return (null);
}// ArrayCursor_null


// Connects to the database if its not already.
function db_superConnect()
{
 if (!db_reallyConnected())
   database.connect(project.dbType,project.dbName,project.dbUser,project.dbPassword,"");
 if (!db_reallyConnected())
   error("Could not connect to database. - "+database.majorErrorMessage()+" ["+database.minorErrorMessage()+"]");

 return (true);
}// db_superConnect


// Returns true if connected to the database.  The database.connected()
// method doesn't always work so a brute force approach is taken.
function db_reallyConnected()
{
 database.execute("SELECT sysDate FROM DUAL");	// Oracle specific!!!
 return (database.majorErrorCode()==0);
}// db_reallyConnected


// A wrapper for database.cursor().  If the first attempt
// to open the cursor fails, the function tries to reconnect
// and go again.  Returns null if it fails the second time.
// The second argument "updatable" is optional.  When
// set to true a normal updatable cursor is returned. Otherwise
// an ArrayCursor is returned.
function db_superCursor(statement,updatable)
{
 var cursor, upd = false;

 if (arguments.length == 2)
   upd=updatable;

 cursor = database.cursor(statement,upd);
 if (database.majorErrorCode()!=0){		// If something went wrong, try again.
   db_superConnect();
   cursor = database.cursor(statement,upd);
 }
 if (database.majorErrorCode()!=0)		// Still have a problem?
   return (null);

 if (!upd){
   debug("Using array cursor!\n");
   var ac = new ArrayCursor(cursor);
   cursor.close();
   return (ac);
 }else{
   return (cursor);
 }
}// db_superCursor


// Kinda self explanatory!
function initDBPrototypes()
{
 Cursor.prototype.validData = ArrayCursor_validData;
 Cursor.prototype.assign = ArrayCursor_assign;

 String.prototype.stripToASCII7 = String_stripToASCII7;

 DbBuiltin.prototype.superConnect = db_superConnect;
 DbBuiltin.prototype.superCursor = db_superCursor;
 DbBuiltin.prototype.reallyConnected = db_reallyConnected;
}// initDBPrototypes


