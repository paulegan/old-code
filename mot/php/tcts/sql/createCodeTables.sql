CREATE TABLE sending_codes_tb (
  code          VARCHAR2(50)  NOT NULL,
  name          VARCHAR2(100) NOT NULL,
  country	VARCHAR2(3) NOT NULL,
  -- Setup the constraint and index
  CONSTRAINT SENDING_CODES_PK PRIMARY KEY (CODE,COUNTRY)
    USING INDEX
        STORAGE(initial 5K next 5K pctincrease 0)
        TABLESPACE WWW_GENERAL_IND02
)
  STORAGE (initial 5K next 5K pctincrease 0)
  PCTFREE 20
  TABLESPACE WWW_GENERAL_DATA02
  ;

alter table sending_codes_tb add 
 CONSTRAINT sending_codes_FK01 FOREIGN KEY (COUNTRY)
	REFERENCES center_codes_TB;

create PUBLIC SYNONYM SENDING_CODES FOR SENDING_CODES_TB;

CREATE TABLE transaction_codes_tb (
  code		VARCHAR2(50)  NOT NULL,
  name		VARCHAR2(100) NOT NULL,
  country	VARCHAR2(3) NOT NULL,
  -- Setup the constraint and index 
  CONSTRAINT TRANSACTION_CODES_PK PRIMARY KEY (CODE,COUNTRY) 
    USING INDEX 
	STORAGE(initial 5K next 5K pctincrease 0)
	TABLESPACE WWW_GENERAL_IND02
)
  STORAGE (initial 5K next 5K pctincrease 0)
  PCTFREE 20
  TABLESPACE WWW_GENERAL_DATA02
  ;

alter table transaction_codes_tb add 
 CONSTRAINT transaction_codes_FK01 FOREIGN KEY (COUNTRY)
	REFERENCES center_codes_TB;
	
create PUBLIC SYNONYM TRANSACTION_CODES FOR TRANSACTION_CODES_TB;

CREATE TABLE rejection_codes_tb (
  code          VARCHAR2(50)  NOT NULL,
  name          VARCHAR2(100) NOT NULL,
  country	VARCHAR2(3) NOT NULL,  
  -- Setup the constraint and index  
  CONSTRAINT REJECTION_CODES_PK PRIMARY KEY (CODE,COUNTRY) 
    USING INDEX 
	STORAGE(initial 5K next 5K pctincrease 0)
	TABLESPACE WWW_GENERAL_IND02
)
  STORAGE (initial 5K next 5K pctincrease 0)
  PCTFREE 20
  TABLESPACE WWW_GENERAL_DATA02
  ;
  
alter table rejection_codes_tb add 
 CONSTRAINT rejection_codes_FK01 FOREIGN KEY (COUNTRY)
	REFERENCES center_codes_TB;

create PUBLIC SYNONYM REJECTION_CODES FOR REJECTION_CODES_TB;

CREATE TABLE center_codes_tb (
  code          VARCHAR2(50)  NOT NULL,
  name          VARCHAR2(100) NOT NULL,    
  -- Setup the constraint and index
  CONSTRAINT CENTER_CODES_PK PRIMARY KEY (CODE)
    USING INDEX
        STORAGE(initial 5K next 5K pctincrease 0)
        TABLESPACE WWW_GENERAL_IND02
)
  STORAGE (initial 5K next 5K pctincrease 0)
  PCTFREE 20
  TABLESPACE WWW_GENERAL_DATA02
  ;
  
alter table rejection_codes_tb add 
 CONSTRAINT rejection_codes_FK01 FOREIGN KEY (COUNTRY)
	REFERENCES center_codes_TB;
	
create public synonym TRANSACTIONS for TRANSACTIONS_TB;
create PUBLIC SYNONYM CENTER_CODES FOR CENTER_CODES_TB;
