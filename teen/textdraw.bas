DECLARE SUB saveScreen ()

'              °°°±±±²²²ÛÛÛ TEXT DRAWING PROGRAM ÛÛÛ²²²±±±°°°

DECLARE SUB mouseDrag (origY%, origX%)
DECLARE SUB selectTheStyle ()

' $INCLUDE: 'lumpy2.bi'

DIM SHARED mouseInfo AS MouseEvent
DIM SHARED charOF(8) AS INTEGER
DIM SHARED style AS INTEGER
DIM SHARED drawBox AS INTEGER
charOF(0) = 176: charOF(1) = 177: charOF(2) = 178: charOF(3) = 219
charOF(4) = 220: charOF(5) = 221: charOF(6) = 222: charOF(7) = 223
charOF(8) = 32: : style = 3

SCREEN 0
CLS
PRINT " SELECT CHR:";
FOR X = 0 TO 7
 PRINT " "; CHR$(charOF(X));
NEXT X
PRINT " E BOX   ERASE ALL                        SAVE  EXIT"

COLOR 2, 0: PRINT STRING$(80, 95)
LOCATE 20, 1: PRINT STRING$(80, 95)
LOCATE 2, 20: PRINT CHR$(23);
COLOR 7, 0

IF MouseInit(0) - 2 THEN PRINT "Sorry no mouse!!": END
CALL MouseTextPtr(0, CHR$(0), 9, CHR$(7))


DO
 CALL MouseGetInfo(mouseInfo)
 IF mouseInfo.buttons THEN
  IF mouseInfo.Y = 1 THEN selectTheStyle
  IF mouseInfo.Y > 2 AND mouseInfo.Y < 20 THEN
   IF drawBox THEN
    origY = mouseInfo.Y: origX = mouseInfo.X
    CALL mouseDrag(origY, origX)
   ELSE
    MouseHide
    LOCATE mouseInfo.Y, mouseInfo.X
    PRINT CHR$(charOF(style))
    MouseShow
   END IF
  END IF
 END IF
 LOCATE 21, 1: PRINT mouseInfo.Y; mouseInfo.X
LOOP UNTIL LEN(INKEY$)

END

SUB mouseDrag (origY, origX)

DO
  CALL MouseGetInfo(mouseInfo)
  IF mouseInfo.Y > 2 AND mouseInfo.Y < 20 THEN
   IF origX < mouseInfo.X THEN theStep = 1 ELSE theStep = -1
   FOR X = origX TO mouseInfo.X STEP theStep
    LOCATE mouseInfo.Y, X: PRINT CHR$(charOF(style))
   NEXT X
   IF origY < mouseInfo.Y THEN theStep = 1 ELSE theStep = -1
   FOR Y = origY TO mouseInfo.Y STEP theStep
    LOCATE Y, mouseInfo.X: PRINT CHR$(charOF(style))
   NEXT Y
  END IF
  LOCATE 21, 1: PRINT mouseInfo.Y; mouseInfo.X
LOOP WHILE mouseInfo.buttons

END SUB

SUB saveScreen

LOCATE 22, 1: INPUT "Name of file: "; fileName$
LOCATE 22, 1: PRINT "Working..."; SPACE$(40)

OPEN fileName$ FOR OUTPUT AS #1
 FOR Y = 3 TO 19
  line$ = ""
  FOR X = 1 TO 79
   line$ = line$ + CHR$(SCREEN(Y, X))
  NEXT X
  PRINT #1, line$
 NEXT Y
CLOSE #1

LOCATE 22, 1: PRINT SPACE$(40)

END SUB

SUB selectTheStyle

 MouseHide
 COLOR 2, 0
 LOCATE 2, (style + 7) * 2: PRINT "_";
 
  SELECT CASE mouseInfo.X
   CASE 14, 16, 18, 20, 22, 24, 26, 28
    style = mouseInfo.X / 2 - 7
   CASE 30
    style = 8
   CASE 32 TO 34
    IF drawBox THEN
     drawBox = FALSE
     LOCATE 2, 32: PRINT "_";
    ELSE drawBox = TRUE
     LOCATE 2, 32: PRINT CHR$(23);
    END IF
   CASE 38 TO 46
    FOR Y = 2 TO 18
     PRINT SPACE$(80);
    NEXT Y
   CASE 71 TO 74
    CALL saveScreen
   CASE 76 TO 80
    MouseHide
    COLOR 7, 0: CLS
    END
   CASE ELSE
    BEEP
  END SELECT

 LOCATE 2, (style + 7) * 2: PRINT CHR$(23);
 COLOR 7, 0
 MouseShow
    LOCATE 21, 70: PRINT style; drawBox;

END SUB

