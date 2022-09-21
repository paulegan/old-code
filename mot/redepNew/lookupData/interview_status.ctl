LOAD DATA INFILE interview_status.dat
REPLACE INTO TABLE interview_status
FIELDS TERMINATED BY "|"
(
  name	CHAR
)
