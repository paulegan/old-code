REM Program to produce Hotel Key Cards
REM Written by Paul Egan on 20-2-1994
REM For D.C.U. Irish School's Computer Programming Competition
 DECLARE SUB RnC ()
 DECLARE SUB Diags ()
 DECLARE FUNCTION Dup (P AS INTEGER)
 DEFINT A-Z
 RANDOMIZE TIMER
CLS
LOCATE 2, 2: INPUT "Enter the number of cards required: ", K
COLOR 23: LOCATE 2, 60: PRINT "WORKING": COLOR 7
DIM SHARED PREV(K) AS INTEGER

FOR J = 0 TO K - 1
START:

REDIM SHARED CARD(7, 3) AS INTEGER      'Array is dimensioned to cover each of
                                        'points on the card + 2 extra rows to
                                        'facilitate the checking of diagonals.

FOR X = 1 TO 30                 'Randomly select "non-holes".
 LET CARD(INT(6 * RND), INT(4 * RND)) = 1
NEXT X

LET CARD(0, 0) = 1: CARD(0, 3) = 0: CARD(5, 3) = 1: CARD(5, 0) = 0
                                'This is to prevent symmetrical repitition.

CALL RnC                        'Check that holes don't form complete rows,
CALL Diags                      'columns or diagonals.

IF Dup(J) = 1 THEN GOTO START   'Checks for repitition and for No. of holes.

IF CARD(0, 3) = 1 THEN LET CARD(0, 3) = 0: CARD(1, 2) = 1
                                'Make sure D1 hasn't been de-holed!


LOCATE 3 + J, 3: PRINT "CARD"; J + 1; ": ";
FOR X = 0 TO 5
 FOR Y = 0 TO 3
  IF CARD(X, Y) = 0 THEN PRINT CHR$(Y + 65); STR$(X + 1); ",";
 NEXT Y
NEXT X                          'Finally display location of holes.

NEXT J
LOCATE 2, 55: PRINT K; "CARDS COMPLETED."

END

DEFSNG A-Z
SUB Diags

REM *************    CHECK DIAGONALS     *************
FOR Z = 0 TO 3 STEP 3
 FOR W = 0 TO 4
   LET C = 0
  FOR X = W TO W + 3
   LET C = C + CARD(X, ABS(X - W - Z))
  NEXT X
   IF C = 0 THEN LET CARD(X - 4, Z) = 1
 NEXT W
NEXT Z

IF CARD(0, 1) + CARD(1, 0) = 0 THEN LET CARD(1, 0) = 1
IF CARD(0, 2) + CARD(1, 3) = 0 THEN LET CARD(1, 3) = 1
IF CARD(0, 2) + CARD(1, 1) + CARD(2, 0) = 0 THEN LET CARD(1, 1) = 1
IF CARD(0, 1) + CARD(1, 2) + CARD(2, 3) = 0 THEN LET CARD(1, 2) = 1

END SUB

FUNCTION Dup (P AS INTEGER)

C = 0: D = 0
FOR X = 0 TO 5
 FOR Y = 0 TO 3
  IF CARD(X, Y) = 0 THEN C = C + X / (Y + 1): LET D = D + 1
 NEXT Y
NEXT X

IF D < 5 THEN LET Dup = 1: EXIT FUNCTION
IF P = 0 THEN EXIT FUNCTION

LET PREV(P) = C
 FOR X = 0 TO P - 1
  IF PREV(X) = PREV(P) THEN LET Dups = 1
 NEXT X
LET Dup = 0

END FUNCTION

SUB RnC

REM *************  CHECK ROWS & COLUMNS  *************
FOR W = 0 TO 1
 FOR X = 0 TO 5 - 2 * W
   LET C = 0
  FOR Y = 0 TO 3 + 2 * W
   LET C = C + CARD(X * (1 - W) + Y * W, Y * (1 - W) + X * W)
  NEXT Y
   IF C = 0 THEN LET CARD(X * (1 - W) + INT(6 * RND) * W, INT(4 * RND) * (1 - W) + X * W) = 1
 NEXT X
NEXT W

END SUB

