
/* Schema objects for COAP SDS V1.0					*/

/* paul@hr.emea.mot.com		19990903				*/



/************************************************************************/
/* it_course - Initiative Training course info. ~10 rows		*/

DROP TABLE it_course;
CREATE TABLE it_course (
	name		VARCHAR2(50) PRIMARY KEY USING INDEX TABLESPACE www_general,
	duration	NUMBER
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ss_name - Skill Set names. ~100 rows					*/

DROP TABLE ss_name;
CREATE TABLE ss_name (
	name		VARCHAR2(50) PRIMARY KEY USING INDEX TABLESPACE www_general
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ss_status - Skill Set status. ~2 rows				*/

DROP TABLE ss_status;
CREATE TABLE ss_status (
	name		VARCHAR2(10) PRIMARY KEY USING INDEX TABLESPACE www_general
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ss_recert - Skill Set recertification pick-list. ~2 rows		*/

DROP TABLE ss_recert;
CREATE TABLE ss_recert (
	name		VARCHAR2(20) PRIMARY KEY USING INDEX TABLESPACE www_general,
	hours		NUMBER
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ca_reason - Corrective action reason. ~5 rows			*/

DROP TABLE ca_reason;
CREATE TABLE ca_reason (
	name		VARCHAR2(20) PRIMARY KEY USING INDEX TABLESPACE www_general
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ca_stage - Corrective action stage. ~5 rows				*/

DROP TABLE ca_stage;
CREATE TABLE ca_stage (
	name		VARCHAR2(10) PRIMARY KEY USING INDEX TABLESPACE www_general
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* ca_outcome - Corrective action outcome. ~5 rows			*/

DROP TABLE ca_outcome;
CREATE TABLE ca_outcome (
	name		VARCHAR2(20) PRIMARY KEY USING INDEX TABLESPACE www_general
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* initiative_training - All key initiative training data. ~20000 rows	*/

DROP TABLE initiative_training;
CREATE TABLE initiative_training (
	rec		NUMBER PRIMARY KEY USING INDEX TABLESPACE www_general,
	cid		VARCHAR2(8) NOT NULL,
	start_date	DATE NOT NULL,
	end_date	DATE,
	course_name	VARCHAR2(50) NOT NULL REFERENCES it_course(name),
	duration	NUMBER NOT NULL
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX initiative_training_cid ON  initiative_training(
        cid
) TABLESPACE www_general;
CREATE SEQUENCE initiative_training_rec INCREMENT BY 1 START WITH 20000;


/************************************************************************/
/* skill_sets - All skill set (COAP cert.) data. ~7000 rows		*/

DROP TABLE skill_sets;
CREATE TABLE skill_sets (
	rec		NUMBER PRIMARY KEY USING INDEX TABLESPACE www_general,
	cid		VARCHAR2(8) NOT NULL,
	start_date	DATE NOT NULL,
	end_date	DATE,
	cert_name	VARCHAR2(50) NOT NULL REFERENCES ss_name(name),
	cert_status	VARCHAR2(10) NOT NULL REFERENCES ss_status(name),
	cert_date	DATE,
	recert		VARCHAR2(20) NOT NULL REFERENCES ss_recert(name),
	hours		NUMBER NOT NULL
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX skill_sets_cid ON skill_sets (
        cid
) TABLESPACE www_general;
CREATE SEQUENCE skill_sets_rec INCREMENT BY 1 START WITH 20000;


/************************************************************************/
/* corrective_action - All corrective action data. ~2000 rows		*/

DROP TABLE corrective_action;
CREATE TABLE corrective_action (
	rec		NUMBER PRIMARY KEY USING INDEX TABLESPACE www_general,
	cid		VARCHAR2(8) NOT NULL,
	start_date	DATE NOT NULL,
	end_date	DATE,
	reason		VARCHAR2(20) NOT NULL REFERENCES ca_reason(name),
	stage		VARCHAR2(10) NOT NULL REFERENCES ca_stage(name),
	team_leader_cid	VARCHAR2(8),
	hr_cid		VARCHAR2(8),
	interview_date	DATE,
	review_date	DATE,
	outcome		VARCHAR2(20) REFERENCES ca_outcome(name)
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX corrective_action_cid ON corrective_action (
        cid
) TABLESPACE www_general;
CREATE SEQUENCE corrective_action_rec INCREMENT BY 1 START WITH 20000;


/************************************************************************/
/* emp_ident - A few fields to identify employees.  Contains all	*/
/*	Easter Inch employees and is refreshed night from the EMEA HR	*/
/*	data mart.  ~2000 rows						*/

DROP TABLE emp_ident;
CREATE TABLE emp_ident (
	cid		VARCHAR2(8) PRIMARY KEY USING INDEX TABLESPACE www_general,
	core_id		VARCHAR2(8),
	name		VARCHAR2(40) NOT NULL,
	scid		VARCHAR2(8),
	manager		CHAR,
	hr		CHAR
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX emp_ident_name ON emp_ident (
        name
) TABLESPACE www_general;


/************************************************************************/
/* user_access - User access privileges ~20 rows			*/

DROP TABLE user_access;
CREATE TABLE user_access (
	user_id			VARCHAR2(8) PRIMARY KEY USING INDEX TABLESPACE www_general,
	initiative_training	CHAR,
	skill_sets		CHAR,
	corrective_action	CHAR,
	super_user		CHAR
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general
STORAGE (
	INITIAL		1K
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);


EXIT;
