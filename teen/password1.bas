REM BALL
SCREEN 1
ON KEY(10) GOSUB VEND: KEY(10) ON
COLOR 0, 1
C1 = 1: C2 = 2
GOSUB AUTO
CIRCLE (233, 16), 18, C2
DIM BALL(99)
GET (213, 1)-(253, 36), BALL
LINE (213, 1)-(253, 36), 0, BF
REM MOVEMOVEMOVEMOVEMOVEMOVEMOVEMOVE
 Y = 6
PLOOP1:
   PUT (213, Y), BALL, PSET
   LINE (213, Y)-(253, Y + 35), 0, BF
   IF Y = 146 THEN GOTO ESC
   Y = Y + 7
 GOTO PLOOP1
ESC:
 PSET (233, 168.5), 0
 FOR ST = .8 TO .3 STEP -.1
  CIRCLE STEP(0, 1.5), 18, C2, , , ST
  CIRCLE STEP(0, 0), 18, 0, , , ST
 NEXT ST
 FOR ST = .4 TO .8 STEP .1
  CIRCLE STEP(0, -1.5), 18, C2, , , ST
  CIRCLE STEP(0, 0), 18, 0, , , ST
 NEXT ST
 Y = 146
QLOOP:
  PUT (213, Y), BALL, PSET
  LINE (213, Y)-(253, Y + 35), 0, BF
  IF Y = 90 THEN GOTO ESC2
  Y = Y - 7
 GOTO QLOOP
REM MOVEMOVEMOVEMOVEMOVEMOVEMOVEMOVE
ESC2:
CIRCLE (233, 100), 18, C2
FOR PAUSE = 1 TO 500: NEXT PAUSE
CIRCLE (233, 100), 18, 0
FOR SR = .8 TO .1 STEP -.1
 CIRCLE (233, 100), 18, C2, , , SR
 CIRCLE (233, 100), 18, 0, , , SR
NEXT SR
CIRCLE (233, 100), 18, C2, , , .01
CIRCLE (233, 100), 18, 0, , , .01
PX = 100: PY = 100
FOR Q = 1 TO 40
 LINE (215, PX)-(251, PY), 0, B
 PX = PX - 1: PY = PY + 1
 LINE (215, PX)-(251, PY), 2, B
NEXT Q
FOR L = 215 TO 69 STEP -1
 LINE (L, PX)-(251, PY), 2, B
 LINE (L, PX)-(251, PY), 0, B
NEXT L
 LINE (L, PX)-(251, PY), 2, B
RX = 252: QX = 251: QY = PX
FOR TRI = 1 TO 100
 LINE (QX, PX)-(QX, QY), 0
 LINE (QX, QY)-(251, QY), 0
 LINE (251, QY)-(QX, PX), 0
 QX = QX - 2: QY = QY + 2: RY = QY
 LINE (QX, PX)-(QX, QY), 2
 LINE (QX, QY)-(251, QY), 2
 LINE (251, QY)-(QX, PX), 2
 LINE (251, RY - 1)-(QX + 1, PX), 1
 IF QY > PY - 1 THEN GOTO PESC
NEXT TRI
PESC:
SX = 251: RY = PX
FOR TRI = 1 TO 100
 LINE (QX, PX)-(QX, QY), 0
 LINE (QX, QY)-(SX, QY), 0
 LINE (SX, QY)-(SX, PY), 0
 LINE (SX, PY)-(QX, PX), 0
 IF RX < 70 THEN GOTO PESC2
 SX = SX - 2: QX = QX - 2: QY = QY + 1: RX = QX
 LINE (QX, PX)-(QX, QY), 2
 LINE (QX, QY)-(SX, QY), 2
 LINE (SX, QY)-(SX, PY), 2
 LINE (SX, PY)-(QX, PX), 2
 LINE (RX + 1, PX)-(SX + 1, PY), 1
 LINE (69, PY)-(SX, PY), 2
NEXT TRI
PESC2:
LOCATE 20, 15: PRINT "      "
SY = PX: RX = SX
FOR TRI = 1 TO 100
 LINE (QX, SY)-(QX, QY), 2
 LINE (QX, QY)-(SX, QY), 2
 LINE (SX, QY)-(SX, PY), 2
 LINE (SX, PY)-(69, SY), 2
 LINE (69, SY)-(QX, SY), 2
 LINE (68, SY - 2)-(SX + 1, PY), 1
 LINE (QX, SY)-(QX, QY), 0
 LINE (QX, QY)-(SX, QY), 0
 LINE (SX, QY)-(SX, PY), 0
 LINE (SX, PY)-(69, SY), 0
 LINE (69, SY)-(QX, SY), 0
 IF SX < 69 THEN GOTO PESC3
 SX = SX - 2: QX = QX - 2
 SY = SY + 2: QY = QY + 2
NEXT TRI
PESC3:
LOCATE 9, 11
PRINT "PASSWORD  PROTECTION"
LINE (71, 61)-(248, 74), 2, B
LINE (72, 62)-(247, 73), 2, B
LOCATE 12, 13: PRINT "Enter  Password:"
LINE (140, 117)-(179, 130), 1, B
LOCATE 16, 19: PRINT "    "
TRY = 0
PREPEAT:
BP = 19
C = -1
GOSUB CORR
LOCATE 16, 19
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
PLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO POUT
GOTO PLOOP
POUT:
 IF X$ = C$ OR X$ = D$ OR X$ = E$ THEN GOSUB CORR
 LOCATE 16, BP
 PRINT CHR$(8)
 BP = BP + 1
 IF BP = 23 AND C <> 4 THEN GOTO INCOR
 IF C = 4 THEN GOTO CORRECT
GOTO PLOOP
INCOR:
 TRY = TRY + 1
 ON TRY GOTO INC1, INC2, INC3, LAST
INC1:
 LOCATE 12, 13
 PRINT "U  guest  rong!!"
 GOTO PAUSE
INC2:
 LOCATE 12, 13
 PRINT "Nothing like it!"
 GOTO PAUSE
INC3:
 LOCATE 12, 13
 PRINT "One more chance!"
 GOTO PAUSE
LAST:
 LOCATE 12, 13
 PRINT "Thats all folks!"
 FOR PAUSE = 1 TO 300: NEXT PAUSE
END
PAUSE:
 FOR PAUSE = 1 TO 400: NEXT PAUSE
 LOCATE 18, 8
 PRINT "                           "
 LOCATE 16, 19
 PRINT "    "
GOTO PREPEAT
CORRECT:
 LOCATE 12, 13
 PRINT "YOU GOT IT RIGHT"
 FOR PAUSE = 1 TO 400: NEXT PAUSE
 IF X$ = "l" THEN GOTO DOSOUT
 PRINT "LIST"
END
DOSOUT:
CLS
PRINT "DOS"
CHDIR "C:\"
END
CORR:
 C = C + 1
 IF C = 0 THEN LET C$ = "o": D$ = "O": E$ = "p"
 IF C = 1 THEN LET C$ = "p": D$ = "P": E$ = "a"
 IF C = 2 THEN LET C$ = "e": D$ = "E": E$ = "u"
 IF C = 3 THEN LET C$ = "n": D$ = "N": E$ = "l"
RETURN
END

AUTO:
CLS
RESTORE PDATA
FOR X = 1 TO 14
 READ X1, Y1, X2, Y2
 LINE (X1, Y1)-(X2, Y2), C1
NEXT X
CIRCLE (137, 102), 15, C1, 3.141592, 6.283185
PAINT (89, 100), C1, C1
PAINT (137, 100), C1, C1
PAINT (185, 100), C1, C1
LOCATE 20, 15
PRINT "BY PAUL EGAN"
RETURN
VEND:
END
PDATA:
DATA 74,115,89,85,89,85,104,115,104,115,74,115,122,102,122,85,122,85,152,85
DATA 152,85,152,102,170,85,200,85,200,85,200,105,200,105,190,105,190,105
DATA 190,115,190,115,180,115,180,115,180,105,180,105,170,105,170,105,170,85
END
