RANDOMIZE TIMER
DEFINT A-Z
DIM FREQ(41, 1) AS DOUBLE, LOOPS AS DOUBLE
CLS

DO
 LOCATE 4, 18: PRINT "READ FILE [1] OR GENORATE NEW NUMBERS [2] ";
 CLEAR : INPUT WHICH: CLS
 FOR X = 1 TO 40: FREQ(X, 0) = X: NEXT X
 ON WHICH GOSUB READFILE, GENORATE
LOOP WHILE WHICH = 1 OR WHICH = 2

END

READFILE:
 COLOR 23: LOCATE 8, 35: PRINT "WORKING ......"
 OPEN "LOTTO.FIL" FOR INPUT AS #1
  DO UNTIL EOF(1)
   INPUT #1, NUMBER
   FREQ(NUMBER, 1) = FREQ(NUMBER, 1) + 1
  LOOP
 CLOSE #1
  GOSUB SORT
RETURN

GENORATE:
 LOCATE 4, 32: INPUT "HOW MANY LOOPS"; LOOPS
 IF LOOPS > 30000 THEN LOOPS = 30000
 T = TIMER

 LOCATE 15, 29: PRINT "ESTIMATED TIME"; INT(LOOPS * .014); "sec."
 LOCATE 16, 25: PRINT "START: "; TIME$
 COLOR 23: LOCATE 8, 35: PRINT "WORKING ......"

 FOR X = 1 TO LOOPS
  N = INT(40 * RND(1) + 1)
  FREQ(N, 1) = FREQ(N, 1) + 1
 NEXT X

  GOSUB SORT

 OPEN "LOTTO.FIL" FOR APPEND AS #1
  PRINT #1, NUMBERS$;
 CLOSE #1

 LOCATE 16, 45: PRINT "END: "; TIME$
 LOCATE 17, 40: PRINT INT(TIMER - T)
RETURN


SORT:
 LOCATE 8, 35: PRINT "SORTING ......": COLOR 7
 DO
    I = 0
  FOR X = 1 TO 36
   IF FREQ(X, 1) < FREQ(X + 1, 1) THEN
    SWAP FREQ(X, 1), FREQ(X + 1, 1)
    SWAP FREQ(X, 0), FREQ(X + 1, 0)
    I = 1
   END IF
  NEXT X
 LOOP WHILE I = 1

 FOR Y = 0 TO 1
  FOR X = 1 TO 18
   LOCATE X, 1 + Y * 10
   PRINT FREQ(X + Y * 18, 0); FREQ(X + Y * 18, 1)
  NEXT X
 NEXT Y

 FOR Y = 1 TO 36
  FOR X = 1 TO 6
   IF FREQ(X, 0) = Y THEN LET NUMBERS$ = NUMBERS$ + STR$(Y) + ","
  NEXT X
 NEXT Y

 LOCATE 8, 35: PRINT "LOTTO NUMBERS:"
 LOCATE 10, 31: PRINT LEFT$(NUMBERS$, LEN(NUMBERS$) - 1)
RETURN

END

