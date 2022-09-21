create table transactions_audit_tb (
	batch_id	VARCHAR2(50)    NOT NULL,
	username	VARCHAR2(50)    NOT NULL,
	ip		VARCHAR2(50)    NOT NULL,
	insertion_date	VARCHAR2(50)    NOT NULL,
	num_of_trans	VARCHAR2(50)    NOT NULL,
	trans_date	DATE		NOT NULL,
	hr_admin	VARCHAR2(8)     NOT NULL,
	trans_center    VARCHAR2(50)    NOT NULL
) 
  STORAGE (initial 50M next 5M pctincrease 0)
  PCTFREE 40
  TABLESPACE WWW_GENERAL_DATA02
  ;

alter table transactions_audit_tb add
 CONSTRAINT TRANSACTIONS_AUDIT_PK PRIMARY KEY (BATCH_ID)
    USING INDEX
       STORAGE(initial 10M next 1M pctincrease 0)
       TABLESPACE WWW_GENERAL_IND02;
