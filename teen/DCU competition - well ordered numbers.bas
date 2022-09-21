REM Three Digit Well Ordered Number Generator
REM Written by Paul Egan on 22-2-1994
REM For D.C.U. Irish Schools' Computer Programming Competition
CLS
LOCATE 2, 8: PRINT "The three digit well ordered numbers are:"

FOR X = 1 TO 7
 FOR Y = 2 TO 8
  FOR Z = 3 TO 9
   IF X < Y AND Y < Z THEN
    LOCATE 4 + NO \ 8, (NO MOD 8 + 1) * 7
    PRINT STR$(X); STR$(Y); STR$(Z); ",";
    LET NO = NO + 1
   END IF
  NEXT Z
 NEXT Y
NEXT X

LOCATE 16, 8: PRINT "The total number is:"; NO

END

