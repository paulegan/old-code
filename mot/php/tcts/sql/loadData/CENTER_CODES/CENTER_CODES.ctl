LOAD DATA
INFILE CENTER_CODES.dat
APPEND
INTO TABLE CENTER_CODES_TB
FIELDS TERMINATED BY '|' TRAILING NULLCOLS
(
  CODE	CHAR,
  NAME  CHAR
)

