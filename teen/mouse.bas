REM mouse
GOTO mouse

CLS
COM(1) ON
 ON COM(1) GOSUB mouse
 DO: LOOP WHILE INKEY$ = ""
COM(1) OFF
END

mouse:
OPEN "com1:1200,n,7,1,asc" FOR INPUT AS #1
FOR x = 1 TO 10
 INPUT #1, what$
 PRINT what$
NEXT x
CLOSE #1
COM(1) OFF
END

