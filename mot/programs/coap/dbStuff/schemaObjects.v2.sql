
/************************************************************************/
/* fe_course - Further education course names. ~5 rows			*/

DROP TABLE fe_course;
CREATE TABLE fe_course (
	name		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_general_ind01
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* fe_qualification - Further education qualifications. ~5 rows		*/

DROP TABLE fe_qualification;
CREATE TABLE fe_qualification (
	name		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_general_ind01
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* fe_supplier - Further education suppliers. ~5 rows			*/

DROP TABLE fe_supplier;
CREATE TABLE fe_supplier (
	name		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_general_ind01
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* further_education - All further education data. ~2000 rows		*/

DROP TABLE further_education;
CREATE TABLE further_education (
	rec		NUMBER PRIMARY KEY USING INDEX TABLESPACE www_general_ind01,
	cid		VARCHAR2(8) NOT NULL,
	start_date	DATE NOT NULL,
	end_date	DATE,
	internal	CHAR,
	course_name	VARCHAR2(30) NOT NULL REFERENCES fe_course(name),
	qualification	VARCHAR2(30) NOT NULL REFERENCES fe_qualification(name),
	supplier	VARCHAR2(30) NOT NULL REFERENCES fe_supplier(name),
	sponsored	CHAR,
	cost		NUMBER,
	po_number	VARCHAR2(10),
	hours		NUMBER NOT NULL
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX further_education_cid ON further_education (
        cid
) TABLESPACE www_general_ind01;
CREATE SEQUENCE further_education_rec INCREMENT BY 1 START WITH 20000;



/************************************************************************/
/* jrt_course - Job related training course names. ~5 rows		*/

DROP TABLE jrt_course;
CREATE TABLE jrt_course (
	name		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_general_ind01
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* jrt_supplier - Job related training course names. ~5 rows		*/

DROP TABLE jrt_supplier;
CREATE TABLE jrt_supplier (
	name		VARCHAR2(30) PRIMARY KEY USING INDEX TABLESPACE www_general_ind01
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		1K  
	NEXT		1K
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	KEEP
);


/************************************************************************/
/* job_related_training - All job related training data. ~2000 rows		*/

DROP TABLE job_related_training;
CREATE TABLE job_related_training (
	rec		NUMBER PRIMARY KEY USING INDEX TABLESPACE www_general_ind01,
	cid		VARCHAR2(8) NOT NULL,
	start_date	DATE NOT NULL,
	end_date	DATE,
	internal	CHAR,
	course_name	VARCHAR2(30) NOT NULL REFERENCES jrt_course(name),
	supplier	VARCHAR2(30) REFERENCES jrt_supplier(name),
	trainer		VARCHAR2(8),
	hours		NUMBER NOT NULL
)
PCTFREE 5
PCTUSED 75
TABLESPACE www_general_data01
STORAGE (
	INITIAL		2M  
	NEXT		1M
	MINEXTENTS	1  
	MAXEXTENTS	UNLIMITED
	PCTINCREASE	0
	BUFFER_POOL	DEFAULT
);
CREATE INDEX job_related_training_cid ON job_related_training (
        cid
) TABLESPACE www_general_ind01;
CREATE SEQUENCE job_related_training_rec INCREMENT BY 1 START WITH 20000;


/************************************************************************/
/* user_access - Additional columns for user_access			*/

ALTER TABLE user_access ADD further_education CHAR;
ALTER TABLE user_access ADD job_related_training CHAR;
