LOAD DATA
INFILE SENDING_CODES.dat
APPEND
INTO TABLE SENDING_CODES_TB
FIELDS TERMINATED BY '|' TRAILING NULLCOLS
(
  CODE	CHAR,
  NAME  CHAR,
  COUNTRY NULLIF COUNTRY = BLANKS
)

