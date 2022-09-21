REM PCPlus Pointer
CLS
DO
 INPUT "Please enter screen mode  [7,8,9]:"; SCR%
LOOP UNTIL SCR% > 6 AND SCR% < 10
SCREEN SCR%                 '9=640*350    8=640*200   7=320*200
IF SCR% > 7 THEN LET Xmax = 639 ELSE LET Xmax = 319
IF SCR% < 9 THEN LET Ymax = 199 ELSE LET Ymax = 349
S = 1
PSET (Xmax / 2, Ymax / 2), 7

DO
 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""

 IF A$ = CHR$(0) + "H" THEN     'UP
   IF POINT(3) > 0 THEN PSET STEP(0, 0), 0: PSET STEP(0, -S), 7
 END IF
 IF A$ = CHR$(0) + "P" THEN     'DOWN
   IF POINT(3) < Ymax THEN PSET STEP(0, 0), 0: PSET STEP(0, S), 7
 END IF
 IF A$ = CHR$(0) + "K" THEN     'LEFT
   IF POINT(2) > 0 THEN PSET STEP(0, 0), 0: PSET STEP(-S, 0), 7
 END IF
 IF A$ = CHR$(0) + "M" THEN     'RIGHT
   IF POINT(2) < Xmax THEN PSET STEP(0, 0), 0: PSET STEP(S, 0), 7
 END IF
LOOP UNTIL A$ = CHR$(27)
END

