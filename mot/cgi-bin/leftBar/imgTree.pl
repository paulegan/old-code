
# Author	: maria@hr.emea.mot.com 	
# DevDate	: 19980916
# TestedWith	:
# TestDate	: 
# ReviewedBy	: 
# ReviewDate	: 
# ProdDate	:
# History	: 19990114,ken,reorganised data warehouse, removed twin section, added subsections to HRIS
# History	: 19981114,joel,Fixed up the data warehouse section
# History	: 19990413,aaron,added org chart to Rewards
# History	: 19990416,aaron,added ukBenefits to Rewards
# History	: 19990419,aaron,added resources to HRIS private
# History	: 19990623,aaron,added upload tool to Data Mart
# History	: 19990625,robert,removed action list from hris private
# ApprovedBy	:
# Description	: Defines all configurables for the left bar generator.  In particular the image tree.
# Notes		: The tree must be updated for each new section in the site!
# Keywords	:

$workingDir		= "/u04/www/programs/cgi-bin/leftBar";
$leftBarPat		= contentsOfFile("$workingDir/leftBar.pat");

$greySuffix		= "_G";
$highlightSuffix	= "_W";
$preloadImgPat		= "    #IMAGE##NR# = preloadImage(\"/images/bar/#IMAGE##NR#.gif\");\n    #IMAGE##HL# = preloadImage(\"/images/bar/#IMAGE##HL#.gif\");\n";
$linkPat		= " <A HREF=\"#URI#\" TARGET=\"_top\" onMouseOver=\"if (browserSupportsJSImages) document['#IMAGE#'].src=#IMAGE##HL#.src\" onMouseOut=\"if (browserSupportsJSImages) document['#IMAGE#'].src=#IMAGE##NR#.src\"><IMG SRC=\"/images/bar/#IMAGE##NR#.gif\" NAME=\"#IMAGE#\" ALT=\"#IMAGE#\" BORDER=0 HEIGHT=20 WIDTH=130></A><BR>\n";

%imageTree = (
  root		=> [ '',		'/',							'siteMap home renewal docrep calendar HRminutes HRIS MU EAP rewards IDE org DW news' ],
  siteMap	=> [ 'root',		'/siteMap.shtml',					'' ],
  home		=> [ 'root',		'/index.shtml',						'' ],
  docrep	=> [ 'root',		'/documents/',						'' ],
  calendar	=> [ 'root',		'/tools/calendar/',						'' ],
  HRminutes		=> [ 'root',		'/hrminutes/',						'' ],
  HRIS		=> [ 'root',		'/hris/',			' killarney outageschedule oracleHR support y2k misc webTeam private' ],
  outageschedule	=> [ 'HRIS',		'/hris/outage_schedule_1999.shtml', 					'' ],
  oracleHR	=> [ 'HRIS',		'/hris/oracleHR/',					'' ],
  killarney	=> [ 'HRIS',		'/hris/killarney/',					'' ],
  support	=> [ 'HRIS',		'/hris/support/',  					'' ],
  y2k		=> [ 'HRIS',		'/hris/y2000/',						'' ],
  misc		=> [ 'HRIS',		'/hris/misc/',  					'' ],
  webTeam	=> [ 'HRIS',		'/hris/webTeam/',					'' ],
  private	=> [ 'HRIS',		'/hris/private/',					'calhris staffdirectory catalog sendpage' ],
  calhris	=> [ 'private',		'/hris/private/calendar/',		'' ],
  catalog	=> [ 'private',		'/hris/private/catalog/',  	'books cds' ],
  books		=> [ 'catalog',		'/hris/private/catalog/books/',  	'' ],
  cds	    	=> [ 'catalog',		'/hris/private/catalog/cds/',  	'' ],
  staffdirectory	=> [ 'private',		'/hris/private/directory.shtml','' ],
  sendpage	=> [ 'private',		'/hris/private/page/',			'' ],
  MU		=> [ 'root',		'/mu/',							'' ],
  EAP		=> [ 'root',		'/eap/',						'' ],
  rewards	=> [ 'root',		'/compAndBen/',				   	'ukbenefits rewardschart' ],
  ukbenefits	=> [ 'ukbenefits',		'/compAndBen/ukbenefitsonline',						'' ],
  rewardschart	=> [ 'rewards',		'/compAndBen/emeaRewards.shtml',						'' ],
  IDE		=> [ 'root',		'/ide/',						'ideForm ideFaq idemrs' ],
  ideForm	=> [ 'IDE',		'/ide/form/',						'' ],
  ideFaq	=> [ 'IDE',		'/ide/faq/',						'' ],
  idemrs	=> [ 'IDE',		'/idemrs/',						'' ],
  org		=> [ 'root',		'/org/',						'directory' ],
  directory	=> [ 'org',		'/org/directory/',					'' ],
  news		=> [ 'root',		'/news/',						'stock epbl' ],
  stock		=> [ 'news',		'/news/stock/',						'' ],
  epbl		=> [ 'news',		'/news/epbl/',						'' ],
  DW		=> [ 'root',		'/dataWarehouse/',				
  'DW_Brio DW_Load dataOwnerList DW_RepReq DW_Owner updateTable DW_Docu webReports dagger twin twinplus upload consentforms demoDW' ],
  DW_Brio    	=> [ 'DW',		'/brio',						'' ],
  DW_Load	=> [ 'DW',		'/dataWarehouse/HelpDataLoadReport.shtml',		'' ],
  dataOwnerList => [ 'DW',              '/dataWarehouse/publicList.shtml', 			''],
  DW_Owner	=> [ 'DW',		'/dataOwner/',						'' ],
  DW_Docu	=> [ 'DW',		'/dataWarehouse/docs/',					'' ],
  dagger	=> [ 'DW',		'/dataWarehouse/dagger/',				'daggerFaq download commentBox' ],
  twin		=> [ 'DW',		'/twin/',						'' ],
  twinplus	=> [ 'DW',		'/dataWarehouse/twinplus/',				'' ],
  upload	=> [ 'DW',		'/dataWarehouse/upload/',				'' ],
  consentforms	=> [ 'DW',		'/dataWarehouse/consentforms/',				'' ],
  DW_RepReq	=> [ 'DW',		'/reportRequest/',					'' ],
  updateTable   => [ 'DW',              '/updateTables/',					'' ],
  webReports	=> [ 'DW',		'/daggerWebReports/',					'' ],
  demoDW	=> [ 'DW',		'/dataWarehouse/demoDW/',				'' ],
  daggerFaq	=> [ 'dagger',		'/dataWarehouse/dagger/faq.shtml',			'' ],
  download	=> [ 'dagger',		'/dataWarehouse/dagger/download/',			'' ],
  commentBox	=> [ 'dagger',		'/dataWarehouse/dagger/commentBox/',			'' ],
  renewal	=> [ 'root',		'/renewal/',						'skillsInventory suggestionBox' ],
  skillsInventory =>[ 'renewal',		'/skillsInventory/',					'' ],
  suggestionBox	=> [ 'renewal',		'/renewal/suggestion/',					'' ]
);

1;
