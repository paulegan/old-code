
/* Schema objects for Redeployment System V2				*/

/* paul@hr.emea.mot.com		18.4.1999				*/



/************************************************************************/
/* relocation - simple lookup table. ~10 rows				*/

CREATE TABLE relocation (
	name		VARCHAR2(100) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		5K  
	NEXT		5K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* grade - simple lookup table. ~250 rows				*/

CREATE TABLE grade (
	name		VARCHAR2(20) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		10K  
	NEXT		10K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* function - simple lookup table. ~15 rows				*/

CREATE TABLE function (
	name		VARCHAR2(100) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		10K  
	NEXT		10K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* region - simple lookup table. ~4 rows				*/

CREATE TABLE region (
	name		VARCHAR2(100) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		5K  
	NEXT		5K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* country - lookup table maintaining heirarchy with region. ~50 rows	*/

CREATE TABLE country (
	name		VARCHAR2(100) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx,
	region		VARCHAR2(100) NOT NULL REFERENCES region(name)	
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		20K  
	NEXT		20K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);
CREATE INDEX country_region ON country (
	region
) TABLESPACE www_apps_idx;


/************************************************************************/
/* location - lookup table maintaining heirarchy with country. ~250 rows*/

CREATE TABLE location (
	name		VARCHAR2(100) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx,
	country		VARCHAR2(100) NOT NULL REFERENCES country(name)	
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		20K  
	NEXT		20K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);
CREATE INDEX location_country ON location (
	country
) TABLESPACE www_apps_idx;


/************************************************************************/
/* jobs - contains all data on a job/position. Foreign data is		*/
/*	  denormalized for efficiency. >5000 rows.			*/

CREATE TABLE jobs (
	ref		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx,
	author		VARCHAR2(30) NOT NULL,
	hiring_manager	VARCHAR2(30),
	advertise_date	DATE NOT NULL,
	expiry_date	DATE NOT NULL,
	contact_person  VARCHAR2(50),
	contact_address VARCHAR2(400),
	contact_email	VARCHAR2(50) NOT NULL,
	contact_tel	VARCHAR2(50),
	contact_fax	VARCHAR2(50),
	grade		VARCHAR2(20) NOT NULL REFERENCES grade(name),
	relocation	VARCHAR2(100) NOT NULL REFERENCES relocation(name),
	sector		VARCHAR2(20),
	department	VARCHAR2(20) NOT NULL,
	location	VARCHAR2(100) NOT NULL REFERENCES location(name),
	country		VARCHAR2(100) NOT NULL REFERENCES country(name),
	region		VARCHAR2(100) NOT NULL REFERENCES region(name),
	function	VARCHAR2(100) NOT NULL REFERENCES function(name),
	title		VARCHAR2(200) NOT NULL,
	description	VARCHAR2(4000) NOT NULL,
	experience	VARCHAR2(4000) NOT NULL,
	num_positions	NUMBER NOT NULL 
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		40M  
	NEXT		10M 
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX jobs_foreign ON jobs (
	grade,
	relocation,
	location,
	country,
	region,
	function
) TABLESPACE www_apps_idx;
CREATE INDEX jobs_author ON jobs (
	author
) TABLESPACE www_apps_idx;
CREATE INDEX jobs_manager ON jobs (
	hiring_manager
) TABLESPACE www_apps_idx;
CREATE INDEX jobs_dates ON jobs (
	advertise_date,
	expiry_date
) TABLESPACE www_apps_idx;




/************************************************************************/
/* manager_review - simple lookup table. ~10 rows			*/

CREATE TABLE manager_review (
	name		VARCHAR2(200) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		5K  
	NEXT		5K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* interview_status - simple lookup table. ~10 rows			*/

CREATE TABLE interview_status (
	name		VARCHAR2(200) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		5K  
	NEXT		5K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* offer_status - simple lookup table. ~10 rows				*/

CREATE TABLE offer_status (
	name		VARCHAR2(200) PRIMARY KEY USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps
STORAGE (
	INITIAL		5K  
	NEXT		5K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* applications - one row for each application made to a job. 		*/
/*	>20000 rows							*/

CREATE TABLE applications (
	cid		VARCHAR2(8) NOT NULL,
	ref		VARCHAR2(30) NOT NULL REFERENCES jobs(ref),
	time		DATE NOT NULL,
	email		VARCHAR2(200),
	passwd		VARCHAR2(200),
	name		VARCHAR2(200) NOT NULL,
	position	VARCHAR2(200) NOT NULL,
	months_in_pos	VARCHAR2(200),
	manager		VARCHAR2(200),
	grade		VARCHAR2(200),
	bad_boy		CHAR,
	address		VARCHAR2(200) NOT NULL,
	work_tel	VARCHAR2(200) NOT NULL,
	home_tel	VARCHAR2(200) NOT NULL,
	education	VARCHAR2(4000) NOT NULL,
	career		VARCHAR2(4000) NOT NULL,
	additional_text	VARCHAR2(4000) NOT NULL,
	manager_review	VARCHAR2(200) NOT NULL REFERENCES manager_review(name),
	interview_status VARCHAR2(200) NOT NULL REFERENCES interview_status(name),
	offer_status	VARCHAR2(200) NOT NULL REFERENCES offer_status(name),
	can_continue	CHAR NOT NULL,
	PRIMARY KEY ( cid,ref ) USING INDEX TABLESPACE www_apps_idx
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_apps_idx
STORAGE (
	INITIAL		150M  
	NEXT		20M
	MINEXTENTS	1 
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX applications_time ON applications (
	time
) TABLESPACE www_apps_idx;
CREATE INDEX applications_passwd ON applications (
	passwd
) TABLESPACE www_apps_idx;
CREATE INDEX applications_ref ON applications (
	ref
) TABLESPACE www_apps_idx;


EXIT;
