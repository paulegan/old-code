REM Horizontal Histogram Generator
CLS
DIM Frequency(9) AS INTEGER

LOCATE 3, 3: INPUT "Please enter number of digits : ", N
 IF N > 100 THEN LET N = 100
LOCATE 4, 3: PRINT "Please enter"; N; "digits :"

FOR X = 0 TO N - 1
REDO:
  DO
   A$ = INKEY$
  LOOP WHILE A$ = ""
 IF ASC(A$) < 48 OR ASC(A$) > 57 THEN GOTO REDO
 PRINT A$; ",";
 LET Frequency(VAL(A$)) = Frequency(VAL(A$)) + 1
NEXT X

FOR X = 0 TO 9
 LOCATE 9 + X, 5: COLOR X + 1
 IF Frequency(X) * 2 > 60 THEN M = 60 ELSE M = Frequency(X) * 2
 PRINT X; CHR$(222); STRING$(M, 219); Frequency(X)
NEXT X

END

