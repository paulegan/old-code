REM password
DECLARE SUB BUTTON (X1!, Y1!, X2!, Y2!, HL!, SH!, CL!)
DEF SEG = 0: POKE &H417, 0
IF INKEY$ = CHR$(0) + CHR$(83) THEN END
TYPE PASTYPE
 PAS AS STRING * 28
END TYPE
DIM PAS AS PASTYPE
OPEN "PASS.INI" FOR RANDOM AS #1 LEN = 28
 GET #1, 3, PAS.PAS
CLOSE #1
LET PASWORD$ = RTRIM$(PAS.PAS)
GOSUB LOGO

CALL BUTTON(150, 70, 490, 220, 7, 6, 5)
CALL BUTTON(200, 178, 440, 198, 6, 7, 1)
LOCATE 7, 31: PRINT "PASSWORD PROTECTION"
COLOR 2, 7
LOCATE 9, 31: PRINT "Please enter correct"
LOCATE 10, 31: PRINT "password to continue"

GETPASS:
COLOR 1, 7
PASS$ = "": A$ = "": X = 27
DO
 LET PASS$ = PASS$ + A$
BACK:
 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""
   IF A$ = CHR$(8) THEN
    IF X = 27 THEN GOTO BACK
    LOCATE 14, X - 1: PRINT CHR$(219)
    LET PASS$ = LEFT$(PASS$, LEN(PASS$) - 1)
    LET X = X - 1: GOTO BACK
   END IF
 LOCATE 14, X, 1: PRINT CHR$(8);
 X = X + 1
LOOP UNTIL A$ = CHR$(13) OR X = 55
IF N = 1 THEN RETURN

IF PASS$ <> PASWORD$ THEN
 CALL BUTTON(200, 178, 440, 198, 6, 7, 1)
 GOSUB MESSAGE
 DO
 LOOP WHILE INKEY$ = ""
 LOCATE 12, 35: PRINT SPACE$(13)
 GOTO GETPASS
END IF

IF COMMAND$ = "CHANGE" THEN GOTO CHANGE
CALL BUTTON(200, 178, 440, 198, 6, 7, 1)
COLOR 2, 7
LOCATE 9, 30: PRINT "    Please enter     "
LOCATE 10, 30: PRINT "     your name:       "
LET N = 1: GOSUB GETPASS
LET LOG$ = PASS$ + "   " + DATE$ + "  " + TIME$
OPEN "PASS.LOG" FOR APPEND AS #2
 PRINT #2, LOG$
CLOSE #2
END

 LOCATE 14, X - 1: PRINT CHR$(219)
 LET PASS$ = LEFT$(PASS$, LEN(PASS$) - 1)
 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""
 LOCATE 14, X - 1: PRINT CHR$(8)
 LET PASS$ = PASS$ + A$
 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""
RETURN

FILE:
RETURN
END

MESSAGE:
LOCATE 12, 35: COLOR 4, 7
SELECT CASE TRY
 CASE 0
  PRINT " INCORRECT"
 CASE 1
  PRINT "WRONG AGAIN!"
 CASE 2
  PRINT "LAST CHANCE!"
 CASE 3
  COLOR 0, 0: CLS
  DO
  LOOP UNTIL INKEY$ = CHR$(0) + "O"
  END
END SELECT
TRY = TRY + 1
RETURN

CHANGE:
CALL BUTTON(200, 178, 440, 198, 6, 7, 1)
COLOR 2, 7
LOCATE 9, 30: PRINT "   Please enter new   "
LOCATE 10, 30: PRINT "      password:       "
COLOR 1, 7
PASS$ = "": A$ = "": X = 27
DO
 LET PASS$ = PASS$ + A$
 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""
 IF A$ = CHR$(8) THEN
  LOCATE 14, X - 1: PRINT CHR$(219)
  LET PASS$ = LEFT$(PASS$, LEN(PASS$) - 1)
  LET X = X - 1: GOTO BACK
 END IF
 LOCATE 14, X: PRINT CHR$(8)
 X = X + 1
LOOP UNTIL A$ = CHR$(13) OR X = 55
LET PAS.PAS = PASS$
OPEN "PASS" FOR RANDOM AS #1 LEN = 28
 PUT #1, 3, PAS.PAS
CLOSE #1
END

LOGO:
SCREEN 9      '640 * 350
COLOR 1, 7
 LINE (0, 0)-(640, 350), 1, BF
 PALETTE 5, 7
 PALETTE 6, 56
 PALETTE 7, 63
 PALETTE 8, 0
 PALETTE 9, 8
XP = 640: YP = 210

LET BC = 1: B = 9
PI# = 3.1415927#
S = 65
SR = S / 180 * PI#
ER = (180 - S) / 180 * PI#
X = 1
PSET (XP, YP), BC
CIRCLE STEP(-75, 95), 60, 8, SR, ER
CIRCLE STEP(0, 5), 60, 8, SR + .1, ER - .1
CIRCLE STEP(0, 16), 60, 8, SR + .1, ER - .1
CIRCLE STEP(0, 5), 60, 8, SR + .1, ER - .1
CIRCLE STEP(0, 16), 60, 8, SR + .1, ER - .1
CIRCLE STEP(0, 5), 60, 8, SR, ER
REM 310,380
LINE STEP(-23, -87)-STEP(0, 47), 8
LINE STEP(8, -44)-STEP(0, 37), 8   '258,276
LINE STEP(11, -39)-STEP(0, 37), 8   '300,270
LINE STEP(8, -37)-STEP(0, 37), 8    '320,270
LINE STEP(11, -35)-STEP(0, 37), 8
LINE STEP(8, -40)-STEP(0, 47), 8    '385,276
PAINT STEP(-3, -3), 8, 8
PAINT STEP(-20, -10), 8, 8    '310,150
PAINT STEP(0, -15), 8, 8
PAINT STEP(0, -5), 8, 8
PAINT STEP(-5, 5), 8, 8    '310,255
PAINT STEP(10, 0), 8, 8

PAINT STEP(-10, -10), 4, 8
PAINT STEP(0, 15), B, 8    '260,220
PAINT STEP(10, 0), 14, 8
PAINT STEP(0, -15), 2, 8  '340,120
CIRCLE STEP(13, 13), 1, 6
PAINT STEP(0, 0), 6, 6

PSET (XP, YP), BC
LINE STEP(-101, 55)-STEP(0, 5), 8
LINE STEP(0, 0)-STEP(-7, 2), 8
LINE STEP(0, 0)-STEP(0, -5), 8
LINE STEP(0, 0)-STEP(7, -2), 8
PAINT STEP(-2, 2), 8, 8
DIM BOX(20)
GET STEP(-5, -2)-STEP(7, 7), BOX
PUT STEP(-7, -7), BOX, PSET
FOR X = 1 TO 6
 PUT STEP(0, 7), BOX, PSET
NEXT X
PAINT STEP(4, 4), 8, 8
PAINT STEP(0, -7), B, BC
PAINT STEP(0, -7), B, BC
PAINT STEP(0, -7), 8, BC
PAINT STEP(0, -7), 4, BC
PAINT STEP(0, -7), 4, BC
PAINT STEP(0, -7), 8, BC

PSET (XP, YP), BC
LINE STEP(-111, 55)-STEP(0, 4), 8
LINE STEP(0, 0)-STEP(-6, 2), 8
LINE STEP(0, 0)-STEP(0, -4), 8
LINE STEP(0, 0)-STEP(6, -2), 8
PAINT STEP(-2, 2), 8, 8
GET STEP(-4, -2)-STEP(6, 6), BOX
PUT STEP(-6, -6), BOX, PSET
FOR X = 1 TO 6
 PUT STEP(0, 7), BOX, PSET
NEXT X
PAINT STEP(4, -4), B, BC
PAINT STEP(0, -7), B, BC
PAINT STEP(0, -14), 4, BC
PAINT STEP(0, -7), 4, BC

PSET (XP, YP), BC
LINE STEP(-120, 54)-STEP(0, 3), 8
LINE STEP(0, 0)-STEP(-5, 2), 8
LINE STEP(0, 0)-STEP(0, -3), 8
LINE STEP(0, 0)-STEP(5, -2), 8
PAINT STEP(-2, 2), 8, 8
GET STEP(-3, -2)-STEP(5, 5), BOX
PUT STEP(-5, -5), BOX, PSET
FOR X = 1 TO 6
 PUT STEP(0, 7), BOX, PSET
NEXT X
PAINT STEP(3, -3), B, BC
PAINT STEP(0, -7), B, BC
PAINT STEP(0, -14), 4, BC
PAINT STEP(0, -7), 4, BC

PSET (XP, YP), BC
LINE STEP(-128, 53)-STEP(0, 2), 8
LINE STEP(0, 0)-STEP(-4, 2), 8
LINE STEP(0, 0)-STEP(0, -2), 8
LINE STEP(0, 0)-STEP(4, -2), 8
PAINT STEP(-1, 1), 8, 8
PAINT STEP(-1, 1), 8, 8
GET STEP(-2, -2)-STEP(4, 4), BOX
PUT STEP(-4, -4), BOX, PSET
FOR X = 1 TO 6
 PUT STEP(0, 7), BOX, PSET
NEXT X
PAINT STEP(3, -5), B, BC
PAINT STEP(0, -7), B, BC
PAINT STEP(0, -14), 4, BC
PAINT STEP(0, -7), 4, BC
RETURN

SUB BUTTON (X1, Y1, X2, Y2, HL, SH, CL)
LINE (X1, Y1)-(X2, Y2), CL, BF
FOR A = 0 TO 2
 LINE (X1 + A, Y2 - A)-(X1 + A, Y1 + A), HL
 LINE STEP(0, 0)-(X2 - A, Y1 + A), HL
 LINE (X1 + A, Y2 - A)-(X2 - A, Y2 - A), SH
 LINE STEP(0, 0)-(X2 - A, Y1 + A), SH
NEXT A
END SUB
