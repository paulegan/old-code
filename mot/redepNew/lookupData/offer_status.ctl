LOAD DATA INFILE offer_status.dat
REPLACE INTO TABLE offer_status
FIELDS TERMINATED BY "|"
(
  name	CHAR
)
