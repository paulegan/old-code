CREATE TABLE batches_tb (
 batch_id		VARCHAR2(50)	NOT NULL,
 trans_id		VARCHAR2(50) 	NOT NULL,
 trans_date		DATE 		NOT NULL,
 emp_cid		VARCHAR2(8) 	NOT NULL,
 hr_admin		VARCHAR2(8) 	NOT NULL,
 hr_admin_location	VARCHAR2(6)	NOT NULL,
 hr_admin_country	VARCHAR2(3)	NOT NULL,
 trans_type		VARCHAR2(50) 	NOT NULL,
 trans_center		VARCHAR2(50)    NOT NULL
)
  STORAGE (initial 2M next 1M pctincrease 0)
  PCTFREE 40
  TABLESPACE WWW_GENERAL_DATA02
  ; 

create PUBLIC SYNONYM batches for BATCHES_TB;

alter table batches_tb add 
 CONSTRAINT BATCHES_PK PRIMARY KEY (BATCH_ID,TRANS_ID)
    USING INDEX
       STORAGE(initial 1M next 500K pctincrease 0)
       TABLESPACE WWW_GENERAL_IND02;

alter table batches_tb add
 CONSTRAINT BATCHES_UK_01 UNIQUE (TRANS_ID)
     USING INDEX
        STORAGE(initial 1M next 500K pctincrease 0)
        TABLESPACE WWW_GENERAL_IND02;

alter table batches_tb add
 CONSTRAINT BATCHES_FK_01 FOREIGN KEY (HR_ADMIN,TRANS_CENTER)
        REFERENCES HR_ADMINS_TB;

alter table batches_tb add
 CONSTRAINT BATCHES_FK_02 FOREIGN KEY (TRANS_TYPE,TRANS_CENTER)
        REFERENCES TRANSACTION_CODES_TB;

alter table batches_tb add
 CONSTRAINT BATCHES_FK_03 FOREIGN KEY (TRANS_CENTER)
        REFERENCES CENTER_CODES_TB(CODE);
