LOAD DATA INFILE manager_review.dat
REPLACE INTO TABLE manager_review
FIELDS TERMINATED BY "|"
(
  name	CHAR
)
