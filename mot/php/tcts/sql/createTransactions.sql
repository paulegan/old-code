CREATE TABLE transactions_tb (
 trans_id		VARCHAR2(50) 	NOT NULL,
 trans_date		DATE 		NOT NULL,
 emp_cid		VARCHAR2(8) 	NOT NULL,
 hr_admin		VARCHAR2(8) 	NOT NULL,
 hr_admin_location	VARCHAR2(6)	NOT NULL,
 hr_admin_country	VARCHAR2(3)	NOT NULL,
 trans_type		VARCHAR2(50) 	NOT NULL,
 trans_center		VARCHAR2(50)    NOT NULL,
 status			VARCHAR2(1)	NOT NULL, 
 close_date		DATE,
 reject_reason		VARCHAR2(50),
 closed_by		VARCHAR2(8),
 prev_trans_id		VARCHAR2(50)
)
  STORAGE (initial 50M next 5M pctincrease 0)
  PCTFREE 40
  TABLESPACE WWW_GENERAL_DATA02
  ; 
-- Setup Constraints
alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_PK PRIMARY KEY (TRANS_ID)
    USING INDEX
       STORAGE(initial 10M next 1M pctincrease 0)
       TABLESPACE WWW_GENERAL_IND02;
       
alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_FK_01 FOREIGN KEY (PREV_TRANS_ID)
	REFERENCES TRANSACTIONS_TB;
	
alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_FK_02 FOREIGN KEY (HR_ADMIN,TRANS_CENTER)
	REFERENCES HR_ADMINS_TB;
	
alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_FK_03 FOREIGN KEY (TRANS_TYPE,TRANS_CENTER)
	REFERENCES TRANSACTION_CODES_TB;

alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_FK_04 FOREIGN KEY (TRANS_CENTER)
	REFERENCES CENTER_CODES_TB;
	
alter table transactions_tb add 
 CONSTRAINT TRANSACTIONS_FK_05 FOREIGN KEY (REJECT_REASON,TRANS_CENTER)
	REFERENCES REJECTION_CODES_TB;
		    
create public synonym TRANSACTIONS for TRANSACTIONS_TB;

create or replace trigger set_trans_id
before insert or update of trans_id
on transactions_tb
for each row
BEGIN
  if (:new.trans_id IS NULL) then
     select 'TN'||TRANSACTION_ID_SEQ.nextVal into :new.trans_ID from dual;
  end if;
end;
/

