
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


// Constructor for Jobs class.
function Jobs()
{
 // Properties
 this.tableName = "jobs";
 this.requestAugmented = false;
 this.errorMessage = "";
 this.columns = [
   // 		    col name		form name		null?	type	length	validateOnClient
   new ColumnSpec ( "ref",		"ref",			false,	"text",	30,	false ),
   new ColumnSpec ( "author",		"author",		false,	"text",	30,	false ),
   new ColumnSpec ( "hiring_manager",	"hiringManager",	true,	"text",	30,	true ),
   new ColumnSpec ( "advertise_date",	"advertiseDate",	false,	"date",	0,	false ),
   new ColumnSpec ( "expiry_date",	"expiryDate",		false,	"date",	0,	false ),
   new ColumnSpec ( "contact_person",	"contactPerson",	false,	"text",	50,	true ),
   new ColumnSpec ( "contact_address",	"contactAddress",	false,	"text",	400,	true ),
   new ColumnSpec ( "contact_email",	"contactEmail",		false,	"text",	50,	true ),
   new ColumnSpec ( "contact_tel",	"contactTel",		false,	"text",	50,	true ),
   new ColumnSpec ( "contact_fax",	"contactFax",		false,	"text",	50,	true ),
   new ColumnSpec ( "grade",		"grade",		false,	"list",	20,	false ),
   new ColumnSpec ( "relocation",	"relocation",		false,	"list",	100,	false ),
   new ColumnSpec ( "sector",		"sector",		false,	"text",	20,	true ),
   new ColumnSpec ( "department",	"department",		false,	"text",	20,	true ),
   new ColumnSpec ( "location",		"location",		false,	"list",	100,	false ),
   new ColumnSpec ( "country",		"country",		false,	"text",	100,	false ),
   new ColumnSpec ( "region",		"region",		false,	"text",	100,	false ),
   new ColumnSpec ( "function",		"function",		false,	"list",	100,	false ),
   new ColumnSpec ( "title",		"title",		false,	"text",	200,	true ),
   new ColumnSpec ( "description",	"description",		false,	"text",	4000,	true ),
   new ColumnSpec ( "experience",	"experience",		false,	"text",	4000,	true ),
   new ColumnSpec ( "num_positions",	"numOfPositions",	false,	"number",0,	true )
 ];

 // Methods
 this.match = Jobs_match;
 this.resultInfo = Jobs_resultInfo;
 this.detail = Jobs_detail;
 this.list = Jobs_list;
 this.list2 = Jobs_list2;
 this.find = Jobs_find;
 this.augmentRequest = Jobs_augmentRequest;
 this.validData = Jobs_validData;
 this.uniqueRef = Jobs_uniqueRef;
 this.insert = Jobs_insert;
 this.update = Jobs_update;
 this.writeClientCheck = Jobs_writeClientCheck;

}// Jobs


// Fills out & tidies up the request data. Also checks
// if it has been run before.
function Jobs_augmentRequest(request)
{
 var country, hm;
 var twin = new Twin();

 if (this.requestAugmented) return (request);

 if (request.ref) request.ref = request.ref.toUpperCase();
 request.title = request.title.replace(/['"]/g,"");
 request.advertiseDate = (new DateLists("start")).selected();
 request.expiryDate = (new DateLists("expiry")).selected();

 if (request.author && request.author!="")
   request.author = request.author.toUpperCase();
 else
   request.author = request.auth_user.toUpperCase();

 if ((country = location2country(request.location))!=null){
   request.country = country.NAME;
   request.region = country.REGION;
 }else{
   request.country = null;
   request.region = null;
 }

 if (!request.hiringManager || request.hiringManager==""){
   request.hiringManager = null;
   request.hiringManagerName = null;
   request.hiringManagerCID = null;
 }else{
   if ((hm = twin.emailID2record(request.hiringManager))!=null){
     request.hiringManagerName = hm.NAME;
     request.hiringManagerCID = hm.CID;
   }else{
     request.hiringManagerName = null;
     request.hiringManagerCID = null;
   }
 }

 this.requestAugmented = true;

 return (request);
}// Jobs_augmentRequest


// Returns true or false depending on whether the request data
// breaks any constraints.  Sets errorMessage if false.
function Jobs_validData(request)
{
 var request = this.augmentRequest(request);
 var cols = this.columns;

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

 if (request.advertiseDate.getTime()>request.expiryDate.getTime()){
   this.errorMessage = "The expiry date must be later than the advertise date!";
   return (false);
 }

 if (!isValidEmailAddress(request.contactEmail)){
   this.errorMessage = "The contact email field does not contain a valid email address.";
   return (false);
 }

 if (request.hiringManager && request.hiringManagerCID==null){
   this.errorMessage = "The email ID you specified for the hiring manager is invalid.";
   return (false);
 }

 return (true);
}// Jobs_validData


function Jobs_writeClientCheck()
{

 write("<SCRIPT LANGUAGE=\"JavaScript\">\n");
 write(" <!--\n\n");
 write(" "+isWord.toString()+"\n");
 write(" "+isValidEmailAddress.toString()+"\n");
 write(" "+isMotEmailID.toString()+"\n");

 (new ArrayCursor()).writeClientCheck(this.columns);

 write(" function validatePositionForm(form)\n {\n\n");
 write("  if (!checkFormValues(form)) return (false);\n\n");
 write("  with(form){\n");
 write("    var theStartDate = new Date(startYear.options[startYear.options.selectedIndex].value,startMonth.options[startMonth.options.selectedIndex].value,startDate.options[startDate.options.selectedIndex].value);\n");
 write("    var theExpiryDate = new Date(expiryYear.options[expiryYear.options.selectedIndex].value,expiryMonth.options[expiryMonth.options.selectedIndex].value,expiryDate.options[expiryDate.options.selectedIndex].value);\n");
 write("    if (theStartDate.getTime()>theExpiryDate.getTime())\n");
 write("      { alert (\"Please choose an expiry date that is after the advertise date.\"); return false; }\n");
 write("    if (isMotEmailID(contactEmail.value) && confirm(\"Change Contact email address to \"+contactEmail.value+\"@email.mot.com ?\"))\n");
 write("      contactEmail.value = contactEmail.value+\"@email.mot.com\";\n");
 write("    if (!isValidEmailAddress(contactEmail.value))\n");
 write("      { alert (\"The Contact email field must contain a valid email address!\\neg: XXX111@email.mot.com\"); return false; }\n");
 write("  }\n\n");
 write("  return (true);\n");
 write(" }\n");

 write("\n // -->\n");
 write("</SCRIPT>\n");

 return (true);
}// Jobs_writeClientCheck


// Inserts a new row in the jobs table.  Checks the data beforehand.
// If anything goes wrong sets errorMessage & returns null, otherwise
// returns the new ref.  Ref is the catenation of project.refPrefix &
// the next value from a sequence.
function Jobs_insert(request)
{
 var job,ref;

 if (!this.validData(request))
   return (null);

 project.lock();
 database.beginTransaction();

 ref = database.superCursor("SELECT '"+project.refPrefix+"'||job_ref.nextVal AS next_ref FROM dual");
 ref.next();
 request.ref = ref.NEXT_REF;
 ref.close();

 job = database.superCursor("SELECT * FROM "+this.tableName+" WHERE ref = '"+request.ref+"'",true);

 if (job.next()){
   job.close();
   this.errorMessage = "Reference "+request.ref+" already exists!";
   return (null);
 }

 job.assign(request,this.columns);

 if (job.insertRow(this.tableName) == 0){
   database.commitTransaction();
 }else{
   this.errorMessage = "Transaction rolled back! "+database.majorErrorMessage();
   database.rollbackTransaction();
   request.ref = null;
 }

 job.close();
 project.unlock();

 return (request.ref);
}// Jobs_insert


// Updates a row in the jobs table.  Checks the data beforehand.
// If anything goes wrong sets errorMessage & returns false.
function Jobs_update(request)
{
 var job;

 if (!this.validData(request))
   return (false);

 database.beginTransaction();
 job = database.superCursor("SELECT * FROM "+this.tableName+" WHERE ref = '"+request.ref+"'",true);

 if (!job.next()){
   job.close();
   this.errorMessage = "Unable to open cursor!"+database.errorMessage;
   return (false);
 }

 job.assign(request,this.columns);

 if (job.updateRow(this.tableName) == 0){
   database.commitTransaction();
   job.close();
   return (true);
 }else{
   this.errorMessage = "Transaction rolled back! "+database.majorErrorMessage();
   database.rollbackTransaction();
   job.close();
   return (false);
 }

}// Jobs_update


function Jobs_match(request)
{
 var whereClause = new WhereClause("sysDate BETWEEN advertise_date AND expiry_date+1");
 var refs = new Array();
 var functionCBF = new CheckboxField("function");
 var functionMS = new MultiSelect("function");
 var gradeMS = new MultiSelect("grade");
 var cursor;

 whereClause.addColumnMatch(functionCBF.columnName,functionCBF.selected());
 whereClause.addColumnMatch(functionMS.columnName,functionMS.selected());
 whereClause.addColumnMatch(gradeMS.columnName,gradeMS.selected());
 whereClause.addColumnMatch("region",[request.region]);
 whereClause.addColumnMatch("country",[request.country]);
 whereClause.addColumnMatch("location",[request.location]);
 whereClause.addTextSearch(request.keywords,request.logicOp);
 whereClause.addPostDate(request.timeOfPosting);


 cursor = database.superCursor("SELECT ref FROM "+this.tableName+" "+whereClause.sql);
 while (cursor.next())
   refs.push(cursor.REF);
 cursor.close();

 return (refs);
}// Jobs_match


// 
function Jobs_resultInfo(refs,order)
{
 var outputColumns = "ref,title,SUBSTR(description,0,400) AS description,function,location,country,grade,sector,department,advertise_date,expiry_date"; 
 var whereClause = new WhereClause();

 whereClause.addColumnMatch("ref",refs);

 return (database.superCursor("SELECT "+outputColumns+" FROM "+this.tableName+" "+whereClause.sql+" ORDER BY "+order));
}// Jobs_resultInfo

function Jobs_detail(ref)
{
 var temp = database.superCursor("SELECT * FROM "+this.tableName+" WHERE ref = '"+ref+"'");
 temp.next();
 return (temp);
}// Jobs_detail

function Jobs_list(user,order,recentOnly)
{
 return (database.superCursor("( SELECT j.ref,j.title,j.num_positions,COUNT(a.cid) AS num_applications FROM "+this.tableName+" j , applications a WHERE a.ref(+) = j.ref AND j.author = '"+user.toUpperCase()+"' "+((recentOnly)?"AND j.expiry_date > (sysDate-7)":"")+" GROUP BY j.ref,j.title,j.num_positions ) UNION ( SELECT j.ref,j.title,j.num_positions,COUNT(a.cid) AS num_applications FROM "+this.tableName+" j , applications a WHERE a.ref(+) = j.ref AND j.hiring_manager = '"+user.toUpperCase()+"' "+((recentOnly)?"AND j.expiry_date > (sysDate-7)":"")+" GROUP BY j.ref,j.title,j.num_positions ) ORDER BY "+(order?order:"ref")));
}// Jobs_list

function Jobs_list2(user,order,recentOnly)
{
 return (database.superCursor("SELECT ref,title,function,location,department,advertise_date,expiry_date FROM "+this.tableName+" WHERE ( author = '"+user.toUpperCase()+"' OR hiring_manager = '"+user.toUpperCase()+"' ) "+((recentOnly)?"AND expiry_date > (sysDate-7)":"")+" ORDER BY "+(order?order:"ref")));
}// Jobs_list2

function Jobs_find(ref,author)
{
 var temp = database.superCursor("SELECT ref FROM "+this.tableName+" WHERE UPPER(ref) LIKE '%"+ref.toUpperCase()+"%' AND author = '"+author.toUpperCase()+"'");
 var ref = null;

 if (temp.next())
   ref = temp.REF;
 temp.close();

 return (ref);
}// findJob

function Jobs_uniqueRef(ref)
{
 var temp = database.superCursor("SELECT ref FROM "+this.tableName+" WHERE ref = '"+request.ref+"'");
 var unique = true;

 if (temp.next()) unique = false;
 temp.close();

 return (unique);
}// Jobs_uniqueRef

