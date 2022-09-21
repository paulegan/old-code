LOAD DATA INFILE location.dat
REPLACE INTO TABLE location
FIELDS TERMINATED BY "|"
(
  name		CHAR,
  country	CHAR
)
