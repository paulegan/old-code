LOAD DATA INFILE relocation.dat
REPLACE INTO TABLE relocation
FIELDS TERMINATED BY "|"
(
  name	CHAR
)
