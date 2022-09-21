REM BALL
SCREEN 1
SCORE1 = 5: SCORE2 = 5
START:
ON KEY(11) GOSUB PMOVEU
ON KEY(14) GOSUB PMOVED
ON KEY(1) GOSUB QMOVEU
ON KEY(2) GOSUB QMOVED
ON KEY(10) GOSUB VEND: KEY(10) ON
ON ERROR GOTO PERR
DIM BALL(166)
DIM PAD2(11)
DIM PAD1(11)
K = 1: S = 5: SB = 10: PY = 98: QY = 98
CLS
LINE (1, 9)-(310, 9), 1
LINE (310, 190)-(1, 190), 1
LINE (306, 98)-(311, 118), 2, BF
LINE (14, 98)-(9, 118), 2, BF
GET (306, 98)-(311, 118), PAD2
GET (14, 98)-(9, 118), PAD1
LOCATE 1, 1
PRINT "LIVES:                        LIVES:   "
LOCATE 1, 7: PRINT SCORE2: LOCATE 1, 37: PRINT SCORE1
IF T = 2 THEN GOTO SKIP
 CIRCLE (135, 100), 15, 1
 PAINT (135, 100), 1, 1
 GET (120, 85)-(150, 115), BALL
 PUT (120, 85), BALL
 LOCATE 10, 7
 PRINT "FIRST TO SAVE 4 BALLS WINS!"
 LOCATE 11, 7
 PRINT "  KEYS: UP, DOWN; F1, F2   "
 WHILE INKEY$ = "": WEND
KEY(11) ON: KEY(14) ON
KEY(1) ON: KEY(2) ON
 LOCATE 10, 7
 PRINT "                            "
 LOCATE 11, 7
 PRINT "                            "
SKIP:
X = 160
Y = INT(RND * 100)
A = INT(RND * 100)
IF Y > 85 THEN LET Y = 85
IF Y < 9 THEN LET Y = 9
PUT (X, Y), BALL, OR
IF A < 50 GOTO L2
GOTO L1
LPP:
 IF K = 1 THEN SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
 Z = 1
L1:
 PUT (X, Y), BALL
 X = X + S: Y = Y + S
 PUT (X, Y), BALL, PSET
 IF X > 270 THEN GOTO MINCE
 IF Y > 150 THEN GOTO LPM
GOTO L1
LMP:
 IF K = 1 THEN SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
 W = 1
L2:
 PUT (X, Y), BALL
 X = X - S: Y = Y + S
 PUT (X, Y), BALL, PSET
 IF X < 19 THEN GOTO MINCE2
 IF Y > 150 THEN GOTO LMM
GOTO L2
LPM:
 IF K = 1 THEN SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
 Z = 2
L3:
 PUT (X, Y), BALL
 X = X + S: Y = Y - S
 PUT (X, Y), BALL, PSET
 IF X > 270 THEN GOTO MINCE
 IF Y < 14 THEN GOTO LPP
GOTO L3
LMM:
 IF K = 1 THEN SOUND 130, 2: SOUND 130, 0: SOUND 130, 2
 W = 2
L4:
 PUT (X, Y), BALL
 X = X - S: Y = Y - S
 PUT (X, Y), BALL, PSET
 IF X < 19 THEN GOTO MINCE2
 IF Y < 14 THEN GOTO LMP
GOTO L4
PMOVEU:
PUT (306, PY), PAD2
PY = PY - SB
IF PY < 13 THEN PY = 13
PUT (306, PY), PAD2, PSET
RETURN
PMOVED:
KEY(11) ON
PUT (306, PY), PAD2
PY = PY + SB
IF PY > 168 THEN PY = 168
PUT (306, PY), PAD2, PSET
RETURN
QMOVEU:
PUT (9, QY), PAD1
QY = QY - SB
IF QY < 13 THEN QY = 13
PUT (9, QY), PAD1, PSET
RETURN
QMOVED:
KEY(11) ON
PUT (9, QY), PAD1
QY = QY + SB
IF QY > 168 THEN QY = 168
PUT (9, QY), PAD1, PSET
RETURN
VEND:
END
PRUN:
RUN
PERR:
ON ERROR GOTO L2
GOTO L1
MINCE:
Y2 = Y + 22
Y3 = Y - 5
IF PY > Y3 AND PY < Y2 THEN GOTO BOUNCE
SOUND 40, 10
T = 2
SCORE1 = SCORE1 - 1
LOCATE 1, 37
PRINT SCORE1
IF SCORE1 = 0 THEN GOTO PEND
GOTO START
BOUNCE:
IF Z = 1 GOTO LMP
IF Z = 2 GOTO LMM
END
MINCE2:
Y2 = Y + 22
Y3 = Y - 5
IF QY > Y3 AND QY < Y2 THEN GOTO BOUNCE2
SOUND 40, 10
T = 2
SCORE2 = SCORE2 - 1
LOCATE 1, 7
PRINT SCORE2
IF SCORE2 = 0 THEN GOTO PEND
GOTO START
BOUNCE2:
IF W = 1 GOTO LPP
IF W = 2 GOTO LPM
END
PEND:
LOCATE 11, 12
PRINT "CONGRATULATIONS!!!"
LOCATE 13, 12
IF SCORE1 = 0 THEN PRINT "<<<  Mr. LEFT WINS"
IF SCORE2 = 0 THEN PRINT "Mr. RIGHT WINS >>>"
PLAY "O1L64MLCCCCEEEEFFFFGGGGGGGGN0"
PLAY "O1L64MLCCCCEEEEFFFFGGGGGGGGN0"
PLAY "O1L64MLCCCCEEEEFFFFGGGGGGGG"
PLAY "O1L64MLEEEEEEECCCCCCCEEEEEEEDDDDDDDDDDDD"
PLAY "O1L64MLEEEEDDDDCCCCCCCCCCEEEEEEEEGGGGGGGGN0"
PLAY "O1L64MLGGGGFFFFFFFN0"
PLAY "O1L64MLCCCCEEEEFFFFGGGGGGGG"
PLAY "O1L64MLEEEEEEECCCCCCCDDDDDDDDCCCCCCCCCCCCC"
END

