
/*
 *******************************************************************************
 *
 *  File:		$Source$
 *
 *  Description:	SQL script for creating permission schema for use with
 *			perm.php Perm class.
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
 *  History:		Revision 1.1  2001/01/19 11:49:29  eganp
 *  History:		Initial revision
 *  History:
 *
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
  REFERENCES perm_dataset(name);
  
  
CREATE INDEX perm_idx2 ON perm_tb (
 operation,
 dataset
)
 STORAGE(INITIAL 1M NEXT 1M PCTINCREASE 0)
 TABLESPACE www_general_ind02;
