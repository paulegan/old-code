
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990824
//  TestedWith:
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  Description:        Initializes configurables ( project properties ).
//  Keywords:            
//  Notes:		init() is called by start.html


function AdminTable(name,title)
{
 this.name = name;
 this.title = title;
}


function init()
{

// Database connection stuff:
project.dbType		= "ORACLE";
project.dbName		= "hriswww";
project.dbUser		= "coap";
project.dbPassword	= "c0ap$d$";


project.errorLogFile	= "/www/programs/coap/errorLog";

project.tabs		= [
  //	    name			title			acronym	writeBody
  new Tab ( "initiative_training",	"Initiative Training",	"IT",	writeCoapTab ),
  new Tab ( "skill_sets",		"Skill Sets",		"SS",	writeCoapTab ),
  new Tab ( "corrective_action",	"Corrective Action",	"CA",	writeCoapTab ),
  new Tab ( "further_education",	"Further Education",	"FE",	writeCoapTab ),
  new Tab ( "job_related_training",	"Job Related Training",	"JRT",	writeCoapTab ),
  new Tab ( "super_user",		"Admin",		"ADM",	writeAdminTab )
];

project.adminTables = [
   new AdminTable ( "user_access",	"Access Control" ),
   new AdminTable ( "it_course",	"Initiative Training Courses" ),
   new AdminTable ( "ss_name",		"Skill Set Cert Names" ),
   new AdminTable ( "ss_status",	"Skill Set Status" ),
   new AdminTable ( "ss_recert",	"Skill Set Recertification" ),
   new AdminTable ( "ca_reason",	"Corrective Action Reasons" ),
   new AdminTable ( "ca_stage",		"Corrective Action Stages" ),
   new AdminTable ( "ca_outcome",	"Corrective Action Outcomes" ),
   new AdminTable ( "fe_course",	"Further Education Courses" ),
   new AdminTable ( "fe_qualification",	"Further Education Qualifications" ),
   new AdminTable ( "fe_supplier",	"Further Education Supplier" ),
   new AdminTable ( "jrt_course",	"Job Related Training Courses" ),
   new AdminTable ( "jrt_supplier",	"Job Related Training Suppliers" )
];

project.tableDef = [];
project.tableDef["initiative_training"] = [
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( "rec",	       "rec",	       false,  "number",       8,      false,  "Record Number",	false, null ),
  new ColumnSpec ( "cid",	       "cid",	       false,  "text",         8,      true,   "CID",	       false,  null ),
  new ColumnSpec ( "start_date",       "startDate",    false,  "date",         11,     true,   "Start Date",   true,   mungeDate ),
  new ColumnSpec ( "end_date",         "endDate",      true,   "date",         11,     true,   "End Date",     true,   mungeDate ),
  new ColumnSpec ( "course_name",      "courseName",   false,  "list",         50,     false,  "Course Name",  true,   null ),
  new ColumnSpec ( "duration",         "duration",     false,  "number",       2,      true,   "Duration",     true,   mungeDuration )
];
project.tableDef["skill_sets"] = [
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( "rec",	       "rec",	       false,  "number",       8,      false,  "Record Number",	false, null ),
  new ColumnSpec ( "cid",	       "cid",	       false,  "text",         8,      true,   "CID",	       false,  null ),
  new ColumnSpec ( "start_date",       "startDate",    false,  "date",         11,     true,   "Start Date",   true,   mungeDate ),
  new ColumnSpec ( "end_date",         "endDate",      true,   "date",         11,     true,   "End Date",     true,   mungeDate ),
  new ColumnSpec ( "cert_name",        "certName",     false,  "list",         50,     false,  "Cert Name",    true,   null ),
  new ColumnSpec ( "cert_status",      "certStatus",   false,  "list",         10,     false,  "Status",       true,   null ),
  new ColumnSpec ( "cert_date",        "certDate",     false,  "date",         11,     true,   "Cert Date",    true,   mungeDate ),
  new ColumnSpec ( "recert",	       "recert",       false,  "list",         20,     false,  "Recert?",      true,   mungeRecert ),
  new ColumnSpec ( "hours",	       "hours",        false,  "number",       2,      true,   "Duration",     true,   mungeDuration )
];
project.tableDef["corrective_action"] = [
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( "rec",	       "rec",	       false,  "number",       8,      false,  "Record Number",	false, null ),
  new ColumnSpec ( "cid",	       "cid",	       false,  "text",         8,      true,   "CID",	       false,  null ),
  new ColumnSpec ( "start_date",       "startDate",    false,  "date",         11,     true,   "Start Date",   true,   mungeDate ),
  new ColumnSpec ( "end_date",         "endDate",      true,   "date",         11,     true,   "End Date",     true,   mungeDate ),
  new ColumnSpec ( "reason",	       "reason",       false,  "list",         20,     false,  "Reason",       true,   null ),
  new ColumnSpec ( "stage",	       "stage",        false,  "list",         10,     false,  "Stage",        true,   null ),
  new ColumnSpec ( "team_leader_cid",  "manager",      true,   "text",         8,      true,   "Team Leader",  true,   null ),
  new ColumnSpec ( "hr_cid",	       "hr",	       true,   "text",         8,      true,   "HR",	       true,   null ),
  new ColumnSpec ( "interview_date",   "interviewDate", false, "date",         11,     true,   "Interview Date", true, mungeDate ),
  new ColumnSpec ( "review_date",      "reviewDate",   false,  "date",         11,     true,   "Review Date",  true,   mungeDate ),
  new ColumnSpec ( "outcome",	       "outcome",      false,  "list",         20,     false,  "Outcome",      true,   null )
];
project.tableDef["further_education"] = [
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( "rec",	       "rec",	       false,  "number",       8,      false,  "Record Number",	false, null ),
  new ColumnSpec ( "cid",	       "cid",	       false,  "text",         8,      true,   "CID",	       false,  null ),
  new ColumnSpec ( "start_date",       "startDate",    false,  "date",         11,     true,   "Start Date",   true,   mungeDate ),
  new ColumnSpec ( "end_date",         "endDate",      true,   "date",         11,     true,   "End Date",     true,   mungeDate ),
  new ColumnSpec ( "internal",         "internal",     true,   "text",         1,      false,  "Internal",     true,   null ),
  new ColumnSpec ( "course_name",      "courseName",   false,  "list",         30,     false,  "Course Name",  true,   null ),
  new ColumnSpec ( "qualification",    "qualification",false,  "list",         30,     false,  "Qualification",true,   null ),
  new ColumnSpec ( "supplier",         "supplier",     false,  "list",         30,     false,  "Supplier",     true,   null ),
  new ColumnSpec ( "sponsored",        "sponsored",    true,   "text",         1,      false,  "Motorola Sponsored",     true,   null ),
  new ColumnSpec ( "cost",	       "cost",         true,   "number",       5,      true,   "Cost",         true,   null ),
  new ColumnSpec ( "po_number",	       "po_number",    true,   "text",         10,     true,   "PO Number",    true,   null ),
  new ColumnSpec ( "hours",	       "hours",        false,  "number",       2,      true,   "Duration",     true,   null )
];
project.tableDef["job_related_training"] = [
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( "rec",	       "rec",	       false,  "number",       8,      false,  "Record Number",	false, null ),
  new ColumnSpec ( "cid",	       "cid",	       false,  "text",         8,      true,   "CID",	       false,  null ),
  new ColumnSpec ( "start_date",       "startDate",    false,  "date",         11,     true,   "Start Date",   true,   mungeDate ),
  new ColumnSpec ( "end_date",         "endDate",      true,   "date",         11,     true,   "End Date",     true,   mungeDate ),
  new ColumnSpec ( "internal",         "internal",     true,   "text",         1,      false,  "Internal",     true,   null ),
  new ColumnSpec ( "course_name",      "courseName",   false,  "list",         30,     false,  "Course Name",  true,   null ),
  new ColumnSpec ( "supplier",         "supplier",     true,   "list",         30,     false,  "Supplier",     true,   null ),
  new ColumnSpec ( "trainer",	       "trainer",      true,   "text",         8,      true,   "Trainer",      true,   null ),
  new ColumnSpec ( "hours",	       "hours",        false,  "number",       2,      true,   "Duration",     true,   mungeDuration )
];
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
project.tableDef["it_course"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         50,     true,   "Course Name",  true,   null ),
  new ColumnSpec ( "duration",	       "duration",     true,   "number",       2,      false,  "Duration",     false,  null )
];
project.tableDef["ss_name"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         50,     true,   "Skill Set Name",  true,   null )
];
project.tableDef["ss_status"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         10,     true,   "Status",       true,   null )
];
project.tableDef["ss_recert"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         20,     true,   "Recert?",      true,   null ),
  new ColumnSpec ( "hours",	       "hours",        true,   "number",       2,      false,  "Duration",     false,  null )
];
project.tableDef["ca_reason"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         20,     true,   "Reason",       true,   null )
];
project.tableDef["ca_stage"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         10,     true,   "Stage",        true,   null )
];
project.tableDef["ca_outcome"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         20,     true,   "Outcome",      true,   null )
];
project.tableDef["fe_course"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         30,     true,   "Course Name",  true,   null )
];
project.tableDef["fe_qualification"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         30,     true,   "Qualification",  true,   null )
];
project.tableDef["fe_supplier"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         30,     true,   "Supplier",  true,   null )
];
project.tableDef["jrt_course"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         30,     true,   "Course Name",  true,   null )
];
project.tableDef["jrt_supplier"] = [
  new ColumnSpec ( "name",	       "name",	       false,  "text",         30,     true,   "Supplier",  true,   null )
];
project.tableDef["user_access"]= [
  new ColumnSpec ( "user_id",	       "user_id",      false,  "text",         8,      true,   "User ID",      true,   null )
];

for (var t=0; t<project.tabs.length; t++ )
  project.tableDef["user_access"].push( new ColumnSpec (project.tabs[t].name,project.tabs[t].name,true,"text",1,false,project.tabs[t].name,true,null ) );


project.col2table = new Array();
project.col2table["initiative_training.course_name"]	= "it_course";
project.col2table["skill_sets.cert_name"]		= "ss_name";
project.col2table["skill_sets.cert_status"]		= "ss_status";
project.col2table["skill_sets.recert"]			= "ss_recert";
project.col2table["corrective_action.reason"]		= "ca_reason";
project.col2table["corrective_action.stage"]		= "ca_stage";
project.col2table["corrective_action.outcome"]		= "ca_outcome";
project.col2table["further_education.course_name"]	= "fe_course";
project.col2table["further_education.qualification"]	= "fe_qualification";
project.col2table["further_education.supplier"]		= "fe_supplier";
project.col2table["job_related_training.course_name"]	= "jrt_course";
project.col2table["job_related_training.supplier"]	= "jrt_supplier";

initDBPrototypes();
database.superConnect();

}// init
