LOAD DATA INFILE country.dat
REPLACE INTO TABLE country
FIELDS TERMINATED BY "|"
(
  name		CHAR,
  region	CHAR
)
