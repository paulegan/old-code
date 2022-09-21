
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19980929
//  TestedWith:
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  Description:        A few miscellaneous functions.
//  Keywords:            
//  Notes:


function error(message)
{
 if (client){
   client.errorMessage = message;
   client.errorPage = request.uri;
   redirect("error.html");
 }else{
   debug(message);
 }
}// error

function addToLog(msg)
{
 var logFile = new File(project.errorLogFile);
 var props = [ 'ip', 'auth_user', 'uri', 'agent' ];
 var now = new Date();

 if (!logFile.open("a")) return(false);
 logFile.write(now.getDate()+"/"+(now.getMonth()+1)+"/"+now.getYear()+"-"+now.getHours()+":"+now.getMinutes());
 logFile.write(" \""+msg.replace(/\n/,"")+"\" ");
 while ((p=props.pop())!=null)
   if (request[p])
      logFile.write("\""+request[p]+"\" ");
 logFile.writeln(" ");
 logFile.close();

 return true;
}// addToLog


// Prints the contents of a file. Returns false on error, else true.
function includeFile(filename)
{
 var f = new File(filename)

 if (!f.open("r")) return(false);
 while (!f.eof())
   write(f.readln()+"\n");
 f.close();

 return(true);
}// includeFile


function instanceOf(object,constructor)
{ 
 while (object != null){ 
   if (object == constructor.prototype) 
     return (true); 
   object = object.__proto__; 
 } 
 return (false); 
}

// Returns true is "str" is a valid word.
function isWord(str)
{
 var digits = "0123456789";
 var lowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
 var upperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 var otherChars = "-._";
 var validChars = lowerCaseLetters+upperCaseLetters+digits+otherChars;
 var ch;

 if (str.length<1) return false;
 for(var c=0; c<str.length; c++)
   if (validChars.indexOf(str.charAt(c))==-1)
     return false;

 return true;
}// isWord


// Checks if "str" is a valid email address. Displays an
// error message if not. An email address is taken to
// be a string with no spaces that contains an "@" and
// at least one ".", where the first "." comes after
// the "@".  The characters between the start & "@",
// "@" & "." and "." & end must also form a valid word.
function isValidEmailAddress(str)
{
 var isEmail = false;

 if ((str!=null) || (str!="") || (str.indexOf(" ")==-1)){
   atLoc = str.indexOf("@");
   dotLoc = str.indexOf(".");
   if ((atLoc>0) && (dotLoc>0) && (dotLoc>atLoc)){
     if (isWord(str.substring(0,atLoc)) &&
	 isWord(str.substring(atLoc+1,dotLoc)) &&
	 isWord(str.substring(dotLoc+1,str.length)))
       isEmail = true;
   }
 }

 return isEmail;
}// isValidEmailAddress


// Returns true is the string is a Motorola email ID.
// This is taken to be a one or more letters followed
// by numbers - and maybe more letters!.
function isMotEmailID(str)
{
 var isEmailID = false;
 var digits = "0123456789";
 var letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
 var i,c=0;

 if ((str.length>2)){
   str = str.toUpperCase()
   while((c<str.length) && (letters.indexOf(str.charAt(c))!=-1))
     c++;
   if ((c>0) && (c<str.length) && (digits.indexOf(str.charAt(c))!=-1)){
     while((c<str.length) && (digits.indexOf(str.charAt(c))!=-1))
       c++;
     if (c==str.length){
       isEmailID=true;
     }else{
       while((c<str.length) && (letters.indexOf(str.charAt(c))!=-1))
     	 c++;
       if (c==str.length)
     	 isEmailID=true;
     }
   }
 }

 return isEmailID;
}// isMotEmailID


// Returns true if dateStr matches /^[0-9]{2}-[A-Za-z]{3}-[0-9]{4}$/ (DD-MON-YYYY).
function looksLikeDate(dateStr)
{
 var digits = "0123456789";
 var letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
 var i=0;

 if (dateStr.length != 11) return false;
 
 while (i<2)
   if (digits.indexOf(dateStr.charAt(i++))==-1)
     return false;
 if (dateStr.charAt(i++)!="-") return false;
 while (i<6)
   if (letters.indexOf(dateStr.charAt(i++))==-1)
     return false;
 if (dateStr.charAt(i++)!="-") return false;
 while (i<11)
   if (digits.indexOf(dateStr.charAt(i++))==-1)
     return false;

 return true;
}// looksLikeDate


// Returns true if cid matches /[0-9]{8}/
function looksLikeCID(cid)
{
 if (cid.length != 8) return false;

 for (i=0; i<8; i++)
   if (isNaN(cid.charAt(i)))
     return false;

 return true;
}// looksLikeCID


// Client side function that returns false if the form's
// cid field does not matche /[0-9]{8}/
function validateCIDForm(form)
{
 if (!looksLikeCID(form.cid.value)){
   alert("The CID you have inputed does not seem to be valid!");
   return false;
 }

 return true;
}// validateCIDForm


// A function to print a date in DD-MON-YYYY form.
function mungeDate(dt)
{
 return (dt.toString().replace(/^[^ ]* ([A-Za-z]{3}) ([0-9]{2}) .* ([0-9]{4})$/,"$2-$1-$3").toUpperCase());
}// mungeDate


// Simply adds "hours" to the end of the string.
function mungeDuration(d)
{
 return (d+" hours");
}// mungeDuration


// Converts "Certification" to "No" and "Re-certification" to "yes"
function mungeRecert(r)
{
 return (r.match(/^recert/i)?"Yes":"No");
}// mungeRecert


// Called by the TabForm object to generate the HTML for each
// of the 3 COAP tabs.  First prints the CID select form generated
// by EmpIdent, then displays a table of records for the selected
// employee followed by a button to add a new record. 
// Note: corrective action has its own prototype.
function writeCoapTab()
{
 var empIdent = new EmpIdent();
 if (request.tab == "corrective_action")
   var ct = new CorrectiveAction();
 else
   var ct = new CoapTable(request.tab);

 request.cid = empIdent.writeSelectForm(request.cid);

 write("<BR CLEAR=ALL>\n");

 if (request.cid) {

   if (request.update && !ct.update(request))
     write("<P><FONT SIZE=+1 COLOR=RED>Record update failed! - "+ct.errorMessage+"</FONT></P>\n");

   ct.writeTable("cid = '"+request.cid+"'","start_date",true);

   write("<SCRIPT LANGUAGE=\"JavaScript\">\n <!--\n\n  window.name = \"coap_main\";\n // -->\n</SCRIPT>\n");
   write("<FORM>\n");
   write(" <TABLE BORDER=0 WIDTH=90% CELLSPACING=0 CELLPADDING=0><TR><TD ALIGN=RIGHT><INPUT TYPE=SUBMIT VALUE=\" Add New Record \" onClick=\"window.open('update.html?tab="+request.tab+"&cid="+request.cid+"','coap_update','width=600,height=140,resizable=1,scrollbars=yes'); return false;\"></TD></TR></TABLE>\n");
   write("</FORM>\n");
 }

 return (true);
}// writeCoapTab


// Writes out a form for selecting which table to admin on the
// admin tab.  Uses project.adminTables.
function writeAdminTableSelectForm()
{
 write(" <FORM METHOD=POST NAME=\"tableSelect\">\n");
 write(" <TABLE WIDTH=90% BORDER=1 CELLPADDING=5 CELLSPACING=0><TR><TD>\n");
 write("  <TABLE BORDER=0 WIDTH=100%><TR>\n   <TD ALIGN=LEFT>&nbsp;</TD>\n");
 write("   <TD ALIGN=LEFT><FONT FACE=\"Arial,Helvetica\">Select table to admin:</FONT></TD>\n");
 write("   <TD ALIGN=RIGHT><CENTER>\n");
 write("    <SELECT NAME=\"table\" onChange=\"tableSelect.submit();\">\n");

 for (var t=0; t<project.adminTables.length; t++)
   write("     <OPTION VALUE=\""+project.adminTables[t].name+"\" "+((request.table==project.adminTables[t].name)?"SELECTED":"")+"> "+project.adminTables[t].title+"\n");

 write("    </SELECT>\n");
 write("   </CENTER></TD>\n  </TR></TABLE>\n");
 write(" </TD></TR></TABLE>\n");
 write(" </FORM>\n\n");
 write(" <BR CLEAR=ALL>\n");

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

 if (!request.table || request.table=="user_access") {
   if (request.update && request.update=="all" && !userAccess.updateAll(request))
     write("<P><FONT SIZE=+1 COLOR=RED>User update failed! - "+userAccess.errorMessage+"</FONT></P>\n");
 
   if (request.update && request.update=="add" && !userAccess.update(request))
     write("<P><FONT SIZE=+1 COLOR=RED>User add failed! - "+userAccess.errorMessage+"</FONT></P>\n");

   userAccess.writeUpdateAllForm();
   write(" <BR CLEAR=ALL>\n");
   userAccess.writeUpdateForm();
 }else{
   var pickList = new PickList(request.table);

   if (request.add && !pickList.update(request))
     write("<P><FONT SIZE=+1 COLOR=RED>Pick-list addition failed! - "+pickList.errorMessage+"</FONT></P>\n");

   write("<FORM>\n");
   pickList.writeSelectList("","",true);
   write("</FORM>\n\n<BR CLEAR=ALL>\n\n");
   pickList.writeUpdateForm();
 }

 return (true);
}// writeAdminTab
