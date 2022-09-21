
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19980929
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
project.dbName				= "hris01";
project.dbUser				= "redepV2";
project.dbPassword			= "r3d3p";

// Times: ( in  milliseconds )
project.mSecsInADay                     = 86400000;
project.defaultVacancyLength            = 1209600000;   // a fortnight
project.creationDefaultsLifeTime        = 31104000000;  // a year

// Email address's:
project.contactEmail			= "www@hr.emea.mot.com";
project.systemEmail			= "ios";

// Misc:
project.errorLogFile			= "/www/programs/redepNew/errorLog";
project.refPrefix			= 'R';
project.extCharReplacement		= '*';
project.maxNumOfApplications		= 10;
project.defaultJobsPerPage		= 20;
project.defaultOrder			= "function";
project.textSearchCols			= [ "ref", "title", "description", "experience", "location", "country", "region", "sector", "department", "function" ];

initDBPrototypes();
database.superConnect();

}
