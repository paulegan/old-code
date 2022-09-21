REM SIMCITY FUNDS CHECKER              
REM LUMPY - 27,JUNE,1994

DECLARE SUB EDIT (XLOC%, YLOC%)
TYPE LUMPYTYPE
 Y AS STRING * 1
END TYPE
DIM LUMP AS LUMPYTYPE
DEFLNG A-Z
'ON ERROR GOTO PERR

SCREEN 1
LOCATE 2, 5: PRINT "Map of "; COMMAND$

LET XLOC% = 90: YLOC% = 40
'LINE (XLOC% + 1, YLOC%)-(XLOC% + 121, YLOC% + 99), 3, BF

OPEN COMMAND$ FOR RANDOM AS #1 LEN = 1

 FOR X = 1 TO 120
  FOR Y = 0 TO 99
   LET C = X * 200 + Y * 2 + 3050
   GET #1, C, LUMP
   IF LUMP.Y = CHR$(2) OR LUMP.Y = CHR$(4) THEN
    PSET (X + XLOC%, Y + YLOC%), 1
    ELSE PSET (X + XLOC%, Y + YLOC%), 3
   END IF
  NEXT Y
 NEXT X

CALL EDIT(XLOC%, YLOC%)
LOCATE 20, 2, 1: PRINT "Saving..."

 FOR X = 1 TO 120
  FOR Y = 0 TO 99
   IF POINT(X + XLOC%, Y + YLOC%) = 1 THEN
    LUMP.Y = CHR$(2): C = X * 200 + Y * 2 + 3050
    PUT #1, C, LUMP
   END IF
  NEXT Y
 NEXT X

CLOSE #1

END

PERR:
 IF ERR = 52 THEN PRINT "    USAGE:   FUNDS cityname.ext": END
 PRINT "ERROR TYPE"; ERR; "OCCURED"
END

DEFSNG A-Z
SUB EDIT (XLOC%, YLOC%)

LET X% = XLOC% + 5: Y% = YLOC% + 5
LET PC = POINT(X%, Y%)

DO
 DO
  A$ = INKEY$
  PSET (X%, Y%), PC * (TIMER MOD 2)
 LOOP WHILE A$ = ""
  PSET (X%, Y%), PC

 SELECT CASE A$
  CASE IS = CHR$(0) + CHR$(72) 'UP
   LET Y% = Y% - 1: IF Y% < YLOC% THEN Y% = YLOC%
  CASE IS = CHR$(0) + CHR$(80) 'DOWN
   LET Y% = Y% + 1: IF Y% > YLOC% + 99 THEN Y% = YLOC% + 99
  CASE IS = CHR$(0) + CHR$(75) 'LEFT
   LET X% = X% - 1: IF X% < XLOC% + 1 THEN X% = XLOC% + 1
  CASE IS = CHR$(0) + CHR$(77) 'RIGHT
   LET X% = X% + 1: IF X% > XLOC% + 121 THEN X% = XLOC% + 121
  CASE IS = CHR$(13)
   PSET (X%, Y%), 4 - POINT(X%, Y%)
  CASE IS = CHR$(32)
    IF XO% = 0 THEN XO% = X% ELSE XO% = 0
    YO% = Y%
  CASE ELSE
 END SELECT
 LET PC = POINT(X%, Y%): PSET (X%, Y%), 2
 IF XO% <> 0 THEN LINE (XO%, YO%)-(X%, Y%), 1, B
LOOP UNTIL A$ = CHR$(27)

END SUB

