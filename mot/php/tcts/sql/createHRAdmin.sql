CREATE TABLE hr_admins_tb (
 cid		VARCHAR2(8) NOT NULL,
 NAME           VARCHAR2(50) NOT NULL,
 COUNTRY 	VARCHAR2(3) NOT NULL,
 -- Setup the unique key and index
 CONSTRAINT HR_ADMINS_PK PRIMARY KEY (CID,COUNTRY) 
    USING INDEX 
	STORAGE(initial 5K next 5K pctincrease 0)
	TABLESPACE WWW_GENERAL_IND02
)
  STORAGE (initial 10K next 5K pctincrease 0)
  PCTFREE 20
  TABLESPACE WWW_GENERAL_DATA02
  ; 

alter table hr_admins_tb add 
 CONSTRAINT hr_admins_FK01 FOREIGN KEY (COUNTRY)
	REFERENCES center_codes_TB;
	
create PUBLIC SYNONYM HR_ADMINS for HR_ADMINS_TB;

