
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


function init()
{

// Database connection stuff:
project.dbType				= "ORACLE";
project.dbName				= "";
project.dbUser				= "";
project.dbPassword			= "";

// Misc:
project.errorLogFile			= "/www/programs/template/errorLog";

initDBPrototypes();
database.superConnect();

}
