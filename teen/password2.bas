REM BALL
SCREEN 1
DIM BALL(63)
LET S = 7
EX = 303: WI = 170
EX2 = EX - 15: WI2 = WI - 15
EX3 = EX + 15: WI3 = WI + 15
CLS
COLOR 0, 3
CIRCLE (EX, WI), 15, 1
PAINT (EX, WI), 1, 1
GET (EX2, WI2)-(EX3, WI3), BALL
X = EX2
Y = WI2
GOTO L4
LPP:
 SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
L1:
 X = X + S: Y = Y + S
 IF X > (288) THEN GOTO LMP
 IF Y > (168) THEN GOTO LPM
 PUT (X, Y), BALL, PSET
GOTO L1
LMP:
 SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
L2:
 X = X - S: Y = Y + S
 IF X < (2) THEN GOTO LPP
 IF Y > (168) THEN GOTO LMM
 IF X = 211 THEN GOSUB PAUL
 PUT (X, Y), BALL, PSET
GOTO L2
LPM:
 SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
L3:
 X = X + S: Y = Y - S
 IF X > (288) THEN GOTO LMM
 IF Y < (2) THEN GOTO LPP
 PUT (X, Y), BALL, PSET
GOTO L3
LMM:
 SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
L4:
 X = X - S: Y = Y - S
 IF X < (2) THEN GOTO LPM
 IF Y < (2) THEN GOTO LMP
 PUT (X, Y), BALL, PSET
GOTO L4
VEND:
END
PAUL:
 IF X = 43 THEN RETURN
 IF X = 127 THEN RETURN
 IF Y = 8 THEN RETURN
FOR X = 1 TO 20
 READ X1, Y1, X2, Y2
 LINE (X1, Y1)-(X2, Y2), 2
NEXT X
CIRCLE (137, 102), 15, 2, 3.141592, 6.283185
CIRCLE (263, 124), 16, 2, 4.7123, 1.5707, .25
CIRCLE (263, 134), 18, 2, 4.7123, 1.5707, .35
CIRCLE (263, 124), 5, 2, 4.7123, 1.5707, .25
CIRCLE (263, 134), 6, 2, 4.7123, 1.5707, .35
PAINT (89, 100), 2, 2
PAINT (137, 100), 2, 2
PAINT (185, 100), 2, 2
PAINT (250, 130), 2, 2
PAINT (270, 125), 2, 2
PAINT (270, 135), 2, 2
CIRCLE (233, 100), 17, 2
PAINT (233, 100), 2, 2
COLOR 0, 1
LOCATE 20, 15
PRINT "BY PAUL EGAN"
FOR PAUSE = 1 TO 500: NEXT
LINE (30, 40)-(290, 160), 0, BF
LOCATE 7, 10
PRINT " PASSWORD  PROTECTION"
LINE (70, 45)-(248, 57), 2, B
LOCATE 12, 6
PRINT "Please enter correct password:"
LINE (140, 117)-(179, 130), 1, B
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
 IF X$ = C$ OR X$ = D$ THEN GOSUB CORR
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
 LOCATE 18, 8
 PRINT "Unlucky, you guessed wrong!"
 GOTO PAUSE
INC2:
 LOCATE 18, 10
 PRINT "That's nothing like it!"
 GOTO PAUSE
INC3:
 LOCATE 18, 8
 PRINT "One more chance! Thats all!"
 GOTO PAUSE
LAST:
 LOCATE 18, 8
 PRINT "Sorry but that's all folks!"
 FOR PAUSE = 1 TO 300: NEXT PAUSE
 LINE (35, 115)-(285, 160), 0, BF
 LOCATE 16, 8
 PRINT " TO CONTINUE COMPLETE THE  "
 LOCATE 17, 8
 PRINT "      FOLLOWING GAME"
 FOR PAUSE = 1 TO 400: NEXT PAUSE
 CHAIN "PASSBALL"
END
PAUSE:
 FOR PAUSE = 1 TO 400: NEXT PAUSE
 LOCATE 18, 8
 PRINT "                           "
 LOCATE 16, 19
 PRINT "    "
GOTO PREPEAT
CORRECT:
 LOCATE 18, 7
 PRINT "WELL DONE, YOU GOT IT RIGHT!"
 FOR PAUSE = 1 TO 400: NEXT PAUSE
 CHAIN "LIST"
END
CORR:
 C = C + 1
 IF C = 0 THEN LET C$ = "o": D$ = "O"
 IF C = 1 THEN LET C$ = "p": D$ = "P"
 IF C = 2 THEN LET C$ = "e": D$ = "E"
 IF C = 3 THEN LET C$ = "n": D$ = "N"
RETURN
END
DATA 74,115,89,85,89,85,104,115,104,115,74,115,122,102,122,85,122,85,152,85
DATA 152,85,152,102,170,85,200,85,200,85,200,105,200,105,190,105,190,105
DATA 190,115,190,115,180,115,180,115,180,105,180,105,170,105,170,105,170,85
DATA 240,120,260,120,260,120,250,140,250,140,240,120
DATA 263,120,263,123,263,126,263,131,263,136,263,140

