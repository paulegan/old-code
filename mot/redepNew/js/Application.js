
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971122
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        
//  Keywords:            
//  Notes:		


function Application(cid,ref)
{
 // Properties
 this.tableName = "applications";
 this.cid = cid;
 this.ref = ref;
 this.requestAugmented = false;
 this.errorMessage = "";
 this.columns = [
   // 		    col name		form name	   null?  type	  len   validateOnClient
   new ColumnSpec ( "can_continue",     "canContinue",     false, "text", 1,    false ),
   new ColumnSpec ( "cid",	        "cid",  	   false, "text", 8,    true ),
   new ColumnSpec ( "ref",	        "ref",  	   false, "text", 30,   false ),
   new ColumnSpec ( "time",	        "time", 	   false, "date", 0,    false ),
   new ColumnSpec ( "email",	        "email",	   true,  "text", 200,  true ),
   new ColumnSpec ( "passwd",	        "passwd",	   true,  "text", 200,  true ),
   new ColumnSpec ( "name",	        "name", 	   false, "text", 200,  true ),
   new ColumnSpec ( "position",         "position",	   false, "text", 200,  true ),
   new ColumnSpec ( "months_in_pos",    "monthsInPos",     true,  "text", 200,  true ),
   new ColumnSpec ( "manager",          "manager",	   true,  "text", 200,  true ),
   new ColumnSpec ( "grade",	        "grade",	   true,  "text", 200,  true ),
   new ColumnSpec ( "bad_boy",          "badBoy",	   true,  "text", 1,    false ),
   new ColumnSpec ( "address",          "address",	   false, "text", 200,  true ),
   new ColumnSpec ( "work_tel",         "workTel",	   false, "text", 200,  true ),
   new ColumnSpec ( "home_tel",         "homeTel",	   false, "text", 200,  true ),
   new ColumnSpec ( "education",        "education",	   false, "text", 4000, true ),
   new ColumnSpec ( "career",	        "career",	   false, "text", 4000, true ),
   new ColumnSpec ( "additional_text",  "supportingText",  false, "text", 4000, true ),
   new ColumnSpec ( "manager_review",   "managerReview",   false, "text", 200,  false ),
   new ColumnSpec ( "interview_status", "interviewStatus", false, "text", 200,  false ),
   new ColumnSpec ( "offer_status",     "offerStatus",     false, "text", 200,  false )
 ];

 // Methods
 this.submit = Application_submit;
 this.getResume = Application_getResume;
 this.canContinue = Application_canContinue;
 this.validData = Application_validData;
 this.list = Application_list;
 this.list2 = Application_list2;
 this.update = Application_update;
 this.continueList = Application_continueList;
 this.unrestrictUsers = Application_unrestrictUsers;
 this.restrictUsers = Application_restrictUsers;
 this.writeClientCheck = Application_writeClientCheck;
 this.augmentRequest = Application_augmentRequest;

}// Application


function ApplicationStatus(cont,mesg,passwd)
{
 this.cont = cont;
 this.mesg = mesg;
 this.passwd = passwd;
}// ApplicationStatus


function Application_canContinue()
{
 var mesg = "";
 var count = 0;
 var alreadyApplied = false;
 var canContinue = "N";
 var passwd = "";
 var temp = database.superCursor("SELECT ref,can_continue,passwd FROM "+this.tableName+" WHERE cid = '"+this.cid+"'");

 while(temp.next()){
   if (temp.REF == request.ref) alreadyApplied = true;
   canContinue = temp.CAN_CONTINUE;
   if (temp.PASSWD && temp.PASSWD!="") passwd = temp.PASSWD;
   count++;
 }
 temp.close();

 if ( alreadyApplied ){
   return( new ApplicationStatus(false,"You have already applied for this position!",passwd) );
 }

 if ( canContinue!="Y" ){
   if ( (count>0) && (count%10)==0 )
     return( new ApplicationStatus(false,"You have applied for 10 positions.  If you do not currently have a job within Motorola due to the ISP, you will be unable to apply to further positions until you have talked with your Transition Center consultant.  If you do currently have a job within Motorola, you will be unable to apply to further positions until you have talked with your staffing representative.",passwd) );
   else if ( (count>4) && (count<10) )
     return( new ApplicationStatus(true,"You have applied for five or more positions.  If you do not currently have a job within Motorola due to the ISP, please see your Transition Center consultant.  If you do currently have a job within Motorola, please see your staffing representative.",passwd) );
 }
  
 return( new ApplicationStatus(true,"",passwd) );
}// Application_canContinue


function Application_getResume(passwd)
{
 var temp;

 if (passwd)
   temp = database.superCursor("SELECT * FROM "+this.tableName+" a WHERE a.cid = '"+this.cid+"' AND a.passwd = '"+passwd+"' AND a.time >= ALL ( SELECT time FROM "+this.tableName+" WHERE cid = a.cid AND passwd = '"+passwd+"' )");
 else
   temp = database.superCursor("SELECT * FROM "+this.tableName+" WHERE cid = '"+this.cid+"' AND ref = '"+this.ref+"'");

 if (!temp.next()){
    temp.close();
    return (null);
 }
 return (temp);
}// Application_getResume

function Application_writeClientCheck()
{

 write("<SCRIPT LANGUAGE=\"JavaScript\">\n");
 write(" <!--\n\n");
 write(" "+isWord.toString()+"\n");
 write(" "+isValidEmailAddress.toString()+"\n");
 write(" "+isMotEmailID.toString()+"\n");

 (new ArrayCursor()).writeClientCheck(this.columns);

 write(" function validateApplicationForm(form)\n");
 write(" {\n");
 write("  if (!checkFormValues(form)) return (false);\n\n");
 write("  with(form){\n");
 write("    if (storeResume.checked && (passwd.value==\"\") )\n");
 write("     { alert (\"\\nIf you wish to store your resume you must supply a password.\"); return false; }\n");
 write("    if (email.value!=\"\"){\n");
 write("      if (isMotEmailID(email.value) && confirm(\"Change email address to \"+email.value+\"@email.mot.com ?\"))\n");
 write("	email.value = email.value+\"@email.mot.com\";\n");
 write("      if (!isValidEmailAddress(email.value))\n");
 write("	{ alert (\"\\nThe email field must contain a valid email address!\\neg: XXX111@email.mot.com\"); return false; }\n");
 write("    }\n");
 write("  }\n\n");
 write("  return (true);\n");
 write(" }\n");

 write("\n // -->\n");
 write("</SCRIPT>\n");

 return (true);
}// Application_writeClientCheck


function Application_augmentRequest(request)
{
 if (this.requestAugmented) return (request);

 request.time = new Date();
 request.managerReview = "Unreviewed";
 request.interviewStatus = "Not Interviewed";
 request.offerStatus = "Undecided";
 request.canContinue = "N";
 request.badBoy = (request.pip=="on")?"Y":"N";
 request.passwd = (request.storeResume=="on")?request.passwd:null;

 this.requestAugmented = true;

 return (request);
}// Application_augmentRequest


function Application_submit(request)
{
 var resume,upd=false,result;

 if (!this.validData(request))
   return (false);

 database.beginTransaction();
 resume = database.superCursor("SELECT * FROM "+this.tableName+" WHERE cid = '"+request.cid+"' AND ref = '"+request.ref+"'",true);

 if (resume.next())
   upd=true;

 resume.assign(request,this.columns);

 if (upd)
   result = resume.updateRow(this.tableName);
 else
   result = resume.insertRow(this.tableName);

 if (result == 0){
   database.commitTransaction();
   resume.close();
   return (true);
 }else{
   this.errorMessage = "Transaction rolled back! "+database.majorErrorMessage();
   database.rollbackTransaction();
   resume.close();
   return (false);
 }

}// Application_submit


function Application_validData(request)
{
 var request = this.augmentRequest(request);
 var temp = new ArrayCursor();

 if (!temp.validData(request, this.columns)){
   this.errorMessage = temp.errorMessage;
   return (false);
 }

 if ( (request.storeResume=="on") && (request.passwd=="") ){
   this.errorMessage = "Please supply a password.";
   return (false);
 }

 if ( (request.email!="") && !isValidEmailAddress(request.email)){
   this.errorMessage = "Please supply a valid email address.";
   return (false);
 }

 return (true);
}// Application_validData


function Application_list(ref,order)
{
 return (database.superCursor("SELECT ref,cid,name,manager_review,interview_status,offer_status FROM "+this.tableName+" WHERE ref = '"+ref+"' ORDER BY "+(order?order:"ref")));
}// Application_list

function Application_list2(cid,order)
{
 return (database.superCursor("SELECT j.ref,a.cid,a.name,j.title,a.manager_review,a.interview_status,a.offer_status FROM "+this.tableName+" a, jobs j WHERE a.ref = j.ref AND cid = '"+cid+"' ORDER BY "+(order?order:"ref")));
}// Application_list2


function Application_update()
{
 var reviewSL = new SelectList("manager_review");
 var interviewSL = new SelectList("interview_status");
 var offerSL = new SelectList("offer_status");
 var appl, success=true;

 database.beginTransaction();
 appl = database.superCursor("SELECT manager_review,interview_status,offer_status FROM "+this.tableName+" WHERE cid='"+this.cid+"' AND ref = '"+this.ref+"'",true);
 appl.next();

 if (reviewSL.selected().length)
   appl.MANAGER_REVIEW = reviewSL.selected().pop();
 if (interviewSL.selected().length)
   appl.INTERVIEW_STATUS = interviewSL.selected().pop();
 if (offerSL.selected().length)
   appl.OFFER_STATUS = offerSL.selected().pop();

 if (appl.updateRow("applications") == 0){
   database.commitTransaction();
 }else{
   database.rollbackTransaction();
   success = false;
 }
 appl.close();

 return success;
}// Application_update

function Application_continueList(order)
{
 // return (database.superCursor("SELECT a.cid,a.name,a.can_continue,c.count FROM applications a, ( SELECT cid,COUNT(ref) AS count FROM applications GROUP BY cid ) c WHERE a.cid = c.cid AND a.time >= ALL ( SELECT time FROM applications WHERE cid = a.cid ) AND a.cid in ( SELECT cid FROM ( SELECT cid,COUNT(ref) AS count FROM applications GROUP BY cid ) WHERE MOD(count,10) = 0 ) ORDER BY cid"));
 return (database.superCursor("SELECT a.cid,a.name,a.can_continue,c.count FROM applications a, ( SELECT cid,COUNT(ref) AS count FROM applications GROUP BY cid ) c WHERE a.cid = c.cid AND a.time >= ALL ( SELECT time FROM applications WHERE cid = a.cid ) AND MOD(c.count,10) = 0 ORDER BY "+(order?order:"cid")));
}// Application_continueList

function Application_unrestrictUsers(cidList)
{
 database.beginTransaction();
 user = database.superCursor("SELECT can_continue FROM applications WHERE cid IN ( '"+cidList.join("','")+"' )",true);

 while (user.next()){
   user.CAN_CONTINUE = "Y";
   user.updateRow("applications");
 }

 database.commitTransaction();
 user.close();
}// Application_unrestrictUsers

function Application_restrictUsers(cidList)
{
 database.beginTransaction();
 user = database.superCursor("SELECT can_continue FROM applications WHERE cid IN ( '"+cidList.join("','")+"' )",true);

 while (user.next()){
   user.CAN_CONTINUE = "N";
   user.updateRow("applications");
 }

 database.commitTransaction();
 user.close();
}// Application_restrictUsers
