REM INI
DIM LIN(9) AS STRING
CLS

GOTO INIPUT

OPEN "PASS.INI" FOR INPUT AS #1
 FOR X = 0 TO 9
  LINE INPUT #1, LIN(X)
 NEXT
CLOSE #1
FOR X = 0 TO 9
 PRINT LIN(X)
NEXT
END

INIPUT:
LET PASS$ = "horizon"
PRINT PASS$
FOR X = 1 TO LEN(PASS$)
 MID$(PASS$, X, 1) = CHR$(ASC(MID$(PASS$, X, 1)) + 18)
 PRINT MID$(PASS$, X, 1);
NEXT X

END


REM ASC(MID$(PASS$, X, 1))+18

OPEN "PASS.INI" FOR OUTPUT AS #1
  PRINT #1, LIN(3)
CLOSE #1
