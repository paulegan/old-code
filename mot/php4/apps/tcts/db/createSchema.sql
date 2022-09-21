
/*
 *******************************************************************************
 *
 *  File:		$Source$
 *
 *  Description:	SQL script for creating TCTS schema.
 *
 *  Created:		2001/01/08
 *  Created By:		Paul Egan
 *
 *  Last Edited:	$Date$
 *  Edited By:		$Author$
 *
 *  ID:			$Id$
 *
 *  History:		$Log$
 *  History:		Revision 1.4  2001/02/05 16:16:30  eganp
 *  History:		Added new field - open_date
 *  History:
 *  History:		Revision 1.3  2001/01/25 20:39:03  eganp
 *  History:		Updated description header
 *  History:		
 *  History:		Revision 1.2  2001/01/24 19:33:16  eganp
 *  History:		Index now includes trans_date
 *  History:		
 *  History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 *  History:		Initial import of TCTS3.
 *  History:		
 *
 *******************************************************************************
 */

DROP TABLE trans_tb;
CREATE TABLE trans_tb (
 trans_id	VARCHAR2(10)	NOT NULL,
 batch_id	VARCHAR2(10),
 prev_id	VARCHAR2(10),
 employee	VARCHAR2(10)	NOT NULL,
 type		VARCHAR2(50)	NOT NULL,
 status		VARCHAR2(10)	NOT NULL,
 center		VARCHAR2(20)	NOT NULL,
 last_proc_by	VARCHAR2(10)	NOT NULL,
 submitted_by	VARCHAR2(10)	NOT NULL,
 trans_date	DATE		NOT NULL,
 effective_date	DATE,
 approved_date	DATE,
 close_date	DATE,
 submitted_via	VARCHAR2(50),
 reject_reason	VARCHAR2(60),
 open_date	DATE
)
 STORAGE (INITIAL 5M NEXT 5M PCTINCREASE 0)
 PCTFREE 30
 TABLESPACE www_general_data02; 
CREATE SYNONYM trans FOR trans_tb;

ALTER TABLE trans_tb ADD 
 CONSTRAINT trans_pk PRIMARY KEY (trans_id)
  USING INDEX
   STORAGE(INITIAL 1M NEXT 1M PCTINCREASE 0)
   TABLESPACE www_general_ind02;

CREATE INDEX trans_idx ON trans_tb (
 employee,
 submitted_by,
 type,
 status,
 center,
 trans_date
)
 STORAGE(INITIAL 2M NEXT 2M PCTINCREASE 0)
 TABLESPACE www_general_ind02;

CREATE SEQUENCE trans_id_seq INCREMENT BY 1 START WITH 100000;

CREATE SEQUENCE batch_id_seq INCREMENT BY 1 START WITH 100000;

/*
 *******************************************************************************
 */

DROP TABLE center_tb;
CREATE TABLE center_tb (
 name		VARCHAR(20)	NOT NULL
)
 STORAGE (INITIAL 1K NEXT 1K PCTINCREASE 0)
 PCTFREE 10
 TABLESPACE www_general_data02; 
CREATE SYNONYM center FOR center_tb;

ALTER TABLE center_tb ADD 
 CONSTRAINT center_pk PRIMARY KEY (name)
  USING INDEX
   STORAGE(INITIAL 1K NEXT 1K PCTINCREASE 0)
   TABLESPACE www_general_ind02;

/*
 *******************************************************************************
 */

DROP TABLE type_tb;
CREATE TABLE type_tb (
 name		VARCHAR(50)	NOT NULL,
 center		VARCHAR(20)	NOT NULL
)
 STORAGE (INITIAL 1K NEXT 1K PCTINCREASE 0)
 PCTFREE 10
 TABLESPACE www_general_data02; 
CREATE SYNONYM type FOR type_tb;

ALTER TABLE type_tb ADD 
 CONSTRAINT type_pk PRIMARY KEY (name,center)
  USING INDEX
   STORAGE(INITIAL 1K NEXT 1K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
   
/*
 *******************************************************************************
 */

DROP TABLE reject_reason_tb;
CREATE TABLE reject_reason_tb (
 name		VARCHAR(60)	NOT NULL,
 center		VARCHAR(20)	NOT NULL
)
 STORAGE (INITIAL 1K NEXT 1K PCTINCREASE 0)
 PCTFREE 10
 TABLESPACE www_general_data02; 
CREATE SYNONYM reject_reason FOR reject_reason_tb;

ALTER TABLE reject_reason_tb ADD 
 CONSTRAINT reject_reason_pk PRIMARY KEY (name,center)
  USING INDEX
   STORAGE(INITIAL 1K NEXT 1K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
   
/*
 *******************************************************************************
 */

DROP TABLE submission_type_tb;
CREATE TABLE submission_type_tb (
 name		VARCHAR(50)	NOT NULL,
 center		VARCHAR(20)	NOT NULL
)
 STORAGE (INITIAL 1K NEXT 1K PCTINCREASE 0)
 PCTFREE 10
 TABLESPACE www_general_data02; 
CREATE SYNONYM submission_type FOR submission_type_tb;

ALTER TABLE submission_type_tb ADD 
 CONSTRAINT submission_type_pk PRIMARY KEY (name,center)
  USING INDEX
   STORAGE(INITIAL 1K NEXT 1K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
   
/*
 *******************************************************************************
 */

DROP TABLE status_tb;
CREATE TABLE status_tb (
 code		VARCHAR(10)	NOT NULL,
 name		VARCHAR(10)	NOT NULL,
 center		VARCHAR(20)	NOT NULL
)
 STORAGE (INITIAL 1K NEXT 1K PCTINCREASE 0)
 PCTFREE 10
 TABLESPACE www_general_data02; 
CREATE SYNONYM status FOR status_tb;

ALTER TABLE status_tb ADD 
 CONSTRAINT status_pk PRIMARY KEY (code,center)
  USING INDEX
   STORAGE(INITIAL 1K NEXT 1K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
 
ALTER TABLE trans_tb ADD 
 CONSTRAINT trans_fk_01 FOREIGN KEY (status)
 REFERENCES status_tb(code);

/*
 *******************************************************************************
 */

DROP TABLE perm_operation_tb;
CREATE TABLE perm_operation_tb (
 name		VARCHAR2(50)	NOT NULL
)
 STORAGE (INITIAL 10K NEXT 10K PCTINCREASE 0)
 PCTFREE 40
 TABLESPACE www_general_data02; 
CREATE SYNONYM perm_operation FOR perm_operation_tb;

ALTER TABLE perm_operation_tb ADD 
 CONSTRAINT perm_operation_pk PRIMARY KEY (name)
  USING INDEX
   STORAGE (INITIAL 10K NEXT 10K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
   
/*
 *******************************************************************************
 */

/*
DROP TABLE perm_dataset_tb;
CREATE TABLE perm_dataset_tb (
 name		VARCHAR2(50)	NOT NULL
)
 STORAGE (INITIAL 10K NEXT 10K PCTINCREASE 0)
 PCTFREE 40
 TABLESPACE www_general_data02; 
CREATE SYNONYM perm_dataset FOR perm_dataset_tb;

ALTER TABLE perm_dataset_tb ADD 
 CONSTRAINT perm_dataset_pk PRIMARY KEY (name)
  USING INDEX
   STORAGE (INITIAL 10K NEXT 10K PCTINCREASE 0)
   TABLESPACE www_general_ind02;
*/

CREATE OR REPLACE VIEW perm_dataset AS SELECT name FROM center;
 
/*
 *******************************************************************************
 */


DROP TABLE perm_tb;
CREATE TABLE perm_tb (
 user_id	VARCHAR2(50)	NOT NULL,
 user_name	VARCHAR2(50)	NOT NULL,
 operation	VARCHAR2(50)	NOT NULL,
 dataset	VARCHAR2(50)	NOT NULL
)
 STORAGE (INITIAL 1M NEXT 1M PCTINCREASE 0)
 PCTFREE 40
 TABLESPACE www_general_data02; 
CREATE SYNONYM perm FOR perm_tb;

CREATE INDEX perm_uid_idx ON perm_tb (
  user_id
)
 STORAGE(INITIAL 1M NEXT 1M PCTINCREASE 0)
 TABLESPACE www_general_ind02;


ALTER TABLE perm_tb ADD
 CONSTRAINT perm_fk1 FOREIGN KEY (operation)
  REFERENCES perm_operation(name);
  
ALTER TABLE perm_tb ADD
 CONSTRAINT perm_fk2 FOREIGN KEY (dataset)
  REFERENCES center(name);	-- REFERENCES perm_dataset(name)
  
CREATE INDEX perm_idx2 ON perm_tb (
 operation,
 dataset
)
 STORAGE(INITIAL 1M NEXT 1M PCTINCREASE 0)
 TABLESPACE www_general_ind02;
