
'                  °°°±±±²²²ÛÛÛ    YAHTZEE    ÛÛÛ²²²±±±°°°
'                  °°°±±±²²²ÛÛÛ  Written  By  ÛÛÛ²²²±±±°°°
'                  °°°±±±²²²ÛÛÛ     Lumpy     ÛÛÛ²²²±±±°°°
'                  °°°±±±²²²ÛÛÛ   Dec, 1994   ÛÛÛ²²²±±±°°°

  DECLARE FUNCTION GETKEY% ()
  DECLARE SUB ADDSCORE (CHOICE%, PLAYER%)
  DECLARE SUB BUTTON (STRG$, X1%, Y1%, UD%)

'$INCLUDE: 'lumpy2.bi'
 
  DIM SHARED mousePos AS MouseEvent
  DIM P.ONE(185), P.TWO(185), P.THREE(185), P.FOUR(185), P.FIVE(185), P.SIX(185)
  DIM NAMES$(6), SND%(6), ROLL.AGAIN(5)
  DIM SHARED DICE%(5), SCORE%(18, 7)
  
ProgramStart: 
   DEF SEG = 0: POKE &H417, 64           'CAPS-LOCK ON
   SND%(1) = 49: SND%(2) = 51: SND%(3) = 53
   SND%(4) = 54: SND%(5) = 56: SND%(6) = 61
   RANDOMIZE TIMER
   ON KEY(10) GOSUB SCORING: KEY(10) ON
   ON KEY(3) GOSUB ErrGen: KEY(3) ON
   ON ERROR GOTO PERR

  GOSUB SHOWSCREEN
  
   REDIM BACKRND(2200): GET (38, 69)-(412, 156), BACKRND
   G1Color 0, 0: G1Box 25, 75, 205, 155, 1
   G1Color 2, 0: G1Box 20, 70, 200, 150, 1
   G1Color 1, 0: G1Box 23, 73, 197, 147, 0
   G1Color 3, 0: G1Line 23, 73, 197, 73: G1Line 23, 147, 197, 147
   BFont 0: G1Banner "How many players:", 30, 80, 1, 1
   G1Color 0, 0: G1Box 170, 78, 181, 88, 1
    DO
     PLAY "L64T200N70"
     DO
      A$ = INKEY$
     LOOP WHILE A$ = ""
     IF ASC(A$) = 27 THEN END
    LOOP UNTIL ASC(A$) > 48 AND ASC(A$) < 55
     N% = ASC(A$) - 48: LOCATE 11, 44: PRINT STR$(N%)
   G1Color 3, 0: G1Banner "Name of player  :", 30, 115, 1, 1
   FOR X = 1 TO N%
    G1Color 2, 0: G1Box 145, 115, 160, 121, 1
    G1Color 3, 0: G1Banner STR$(X), 140, 115, 1, 1
    G1Color 0, 0: G1Box 37, 126, 181, 136, 1
    PLAY "L64T200N70": LOCATE 17, 11: INPUT "", NAMES$(X)
     NAMES$(X) = LEFT$(NAMES$(X), 6)
   NEXT X
   PUT (38, 69), BACKRND, PSET: ERASE BACKRND
    LINE (514, 33)-STEP(9, 5), 0
    LINE STEP(0, 0)-STEP(10, -15), 0
   
    MOUSE% = MouseInit(1)
    MouseGraphicsPtr 5, 0, MapHand

     TotalFd = 0: PLAYER% = 0

DO
     CALL MouseHide
     IF PLAYER% THEN LINE (77 + (32 * PLAYER%), 20)-STEP(28, 164), 0, B
       FINISHED = 1
       DO
	 PLAYER% = PLAYER% + 1: IF PLAYER% > N% THEN PLAYER% = 1
	 FOR S = 1 TO 13
	  IF SCORE%(S, PLAYER%) = 0 THEN FINISHED = 0
	 NEXT S
	 IF FINISHED THEN TotalFd = TotalFd + 1
	 IF TotalFd = N% THEN GOTO SCORING      'END - WINNNER
       LOOP WHILE FINISHED
    
     LINE (77 + (32 * PLAYER%), 20)-STEP(28, 164), 1, B
     LOCATE 20, 74: PRINT NAMES$(PLAYER%) + SPACE$(6 - LEN(NAMES$(PLAYER%)))
     CALL MouseShow
    
     FOR X = 1 TO 5: ROLL.AGAIN(X) = 1: NEXT X
     ROLL% = 0

   DO
     FOR X = 1 TO 5
      IF ROLL.AGAIN(X) THEN
       DICE%(X) = INT(6 * RND(1) + 1)
	PLAY "L32T200N" + STR$(SND%(DICE%(X)))
	CALL MouseHide
	 IF DICE%(X) = 1 THEN PUT (399, (X * 33) - 13), P.ONE, PSET
	 IF DICE%(X) = 2 THEN PUT (399, (X * 33) - 13), P.TWO, PSET
	 IF DICE%(X) = 3 THEN PUT (399, (X * 33) - 13), P.THREE, PSET
	 IF DICE%(X) = 4 THEN PUT (399, (X * 33) - 13), P.FOUR, PSET
	 IF DICE%(X) = 5 THEN PUT (399, (X * 33) - 13), P.FIVE, PSET
	 IF DICE%(X) = 6 THEN PUT (399, (X * 33) - 13), P.SIX, PSET
	CALL MouseShow
      END IF
     NEXT X
     ROLL% = ROLL% + 1
  
     IF ROLL% = 1 THEN
      FOR X = 1 TO 5
       LINE (514, (X * 33))-STEP(9, 5), 0
       LINE STEP(0, 0)-STEP(10, -15), 0
       ROLL.AGAIN(X) = 0
      NEXT X
     END IF

     IF ROLL% < 3 THEN
      DO
       X = GETKEY% - 64
       IF X > 0 AND X < 6 THEN
	ROLL.AGAIN(X) = (ROLL.AGAIN(X) + 1) MOD 2
	 CALL MouseHide
	  LINE (514, (X * 33))-STEP(9, 5), ROLL.AGAIN(X)
	  LINE STEP(0, 0)-STEP(10, -15), ROLL.AGAIN(X)
	 CALL MouseShow
       END IF
      LOOP UNTIL X = -51
     END IF
   LOOP UNTIL ROLL% = 3
  
   FOR PAUSE = 1 TO 2000: NEXT PAUSE
   'LOCATE 22, 75: PRINT "BOARD": LOCATE 22, 74: PRINT "NUMBER:"
    DO
     DO
       PLAY "L64T200N70"
	CHOICE% = GETKEY% - 48
       PLAY "L64T200N46"
    
       IF CHOICE% > 9 THEN CHOICE% = CHOICE% - 7
     LOOP WHILE CHOICE% < 1 OR CHOICE% > 13
     IF CHOICE% = 12 AND SCORE%(12, PLAYER%) <> -1 THEN X = 0 ELSE X = SCORE%(CHOICE%, PLAYER%)
    LOOP WHILE X
    
   CALL ADDSCORE(CHOICE%, PLAYER%)

LOOP                    'GOTO NEXTPERSON
    

SCORING:            'END - SORT WINNER
    CALL MouseHide
 
   DO
     SWAPED = 2
     IF N% <> 1 THEN
      FOR X = 1 TO N% - 1
       IF SCORE%(17, X) < SCORE%(17, X + 1) THEN
	 SWAP SCORE%(17, X), SCORE%(17, X + 1)
	 SWAP NAMES$(X), NAMES$(X + 1)
	 SWAPED = 1
       END IF
      NEXT X
     END IF
   LOOP WHILE SWAPED = 1
  
   LINE (300, 20)-(640, 190), 0, BF
   BFont 1: G1Color 2, 0

   IF N% = 6 THEN Y = 5 ELSE Y = N%
   FOR X = 1 TO Y
      IF X = 1 THEN PUT (320, (X * 33) - 17), P.ONE, PSET
      IF X = 2 THEN PUT (320, (X * 33) - 17), P.TWO, PSET
      IF X = 3 THEN PUT (320, (X * 33) - 17), P.THREE, PSET
      IF X = 4 THEN PUT (320, (X * 33) - 17), P.FOUR, PSET
      IF X = 5 THEN PUT (320, (X * 33) - 17), P.FIVE, PSET
    STRG$ = NAMES$(X) + STRING$(10 - LEN(NAMES$(X)), ".") + STR$(SCORE%(17, X))
    G1Banner STRG$, 527, (X * 33) - 6, 1, 1
   NEXT X

     SLEEP 1
     PLAY "L64T200N70"
     LOCATE 23, 55: PRINT "PLAY AGAIN?  Y/N  ";
     IF GETKEY% = 89 THEN
      ERASE SCORE
      GOTO ProgramStart
     END IF
END


SHOWSCREEN:
 SCREEN 1
 G1Mode 1
 WIDTH (80)
 CLS

   BFont 2: G1Color 1, 0
   G1Banner "YAHTZEE", 3, 1, 3, 1
   G1Banner "Lumpy '94", 230, 1, 1, 1

     PRINT : PRINT : PRINT
     PRINT "ACES........1 ... ... ... ... ... ... 1"
     PRINT "TWOS........2 ... ... ... ... ... ... 2"
     PRINT "THREES......3 ... ... ... ... ... ... 3"
     PRINT "FOURS.......4 ... ... ... ... ... ... 4"
     PRINT "FIVES.......5 ... ... ... ... ... ... 5"
     PRINT "SIXES.......6 ... ... ... ... ... ... 6"
     PRINT
     PRINT "TOTAL UPPER.. ... ... ... ... ... ... "
     PRINT
     PRINT "3 OF A KIND.7 ... ... ... ... ... ... 7"
     PRINT "4 OF A KIND.8 ... ... ... ... ... ... 8"
     PRINT "FULL HOUSE..9 ... ... ... ... ... ... 9"
     PRINT "SM STRAIGHT.A ... ... ... ... ... ... A"
     PRINT "LG STRAIGHT.B ... ... ... ... ... ... B"
     PRINT "YAHTZEE.....C ... ... ... ... ... ... C"
     PRINT "CHANCE......D ... ... ... ... ... ... D"
     PRINT
     PRINT "TOTAL LOWER.. ... ... ... ... ... ... "
     PRINT "TOP BONUS.... ... ... ... ... ... ... "
     PRINT "GRAND TOTAL.. ... ... ... ... ... ... "

    PSET (75, 185), 0
    FOR X = 1 TO 7
     LINE STEP(32, -165)-STEP(0, 165), 1
    NEXT X
    LINE STEP(0, 0)-STEP(-192, 0), 1
    LINE STEP(0, -27)-STEP(192, 0), 1
    LINE STEP(0, -67)-STEP(-192, 0), 1
    LINE STEP(0, -16)-STEP(192, 0), 1
    LINE STEP(0, -56)-STEP(-192, 0), 1

  G1Box 292, 63, 319, 134, 1
 G1Color 3, 0
  G1Box 294, 65, 317, 132, 0
  G1Box 293, 64, 318, 133, 0
 G1Color 0, 0
  G1Line 294, 132, 317, 132
  G1Line 293, 133, 318, 133
  G1Line 316, 131, 316, 66
  G1Line 317, 132, 317, 65
  G1Line 318, 133, 318, 64
 G1Banner "R", 299, 70, 2, 1
 G1Banner "O", 299, 85, 2, 1
 G1Banner "L", 299, 100, 2, 1
 G1Banner "L", 299, 115, 2, 1

 G1Color 1, 0: G1Box 200, 87, 241, 117, 1
 FOR X = 0 TO 4
  CIRCLE (442, 102), 80 + X, 0, , , .2
  CIRCLE (442, 102), 42 + X, 0, , , .8
 NEXT X
 GET (399, 86)-STEP(86, 32), P.ONE
 FOR X = 1 TO 5
  PUT (399, (X * 33) - 13), P.ONE, PSET
 NEXT X
 
 G1Color 2, 0
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 221, 36, Z, Y
  NEXT Y
 GET (399, 20)-STEP(86, 32), P.ONE
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 210, 29, Z, Y
  NEXT Y
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 232, 43, Z, Y
  NEXT Y
 GET (399, 20)-STEP(86, 32), P.THREE
 PUT (399, 152), P.THREE, PSET
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 210, 43, Z, Y
  NEXT Y
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 232, 29, Z, Y
  NEXT Y
 GET (399, 20)-STEP(86, 32), P.FIVE
 PUT (399, 119), P.FIVE, PSET
 G1Color 1, 0: G1Box 216, 32, 226, 40, 1: G1Color 2, 0
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 221, 29, Z, Y
  NEXT Y
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 221, 43, Z, Y
  NEXT Y
 GET (399, 20)-STEP(86, 32), P.SIX
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 212, 62, Z, Y
  NEXT Y
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 230, 76, Z, Y
  NEXT Y
 GET (399, 53)-STEP(86, 32), P.TWO
 PUT (399, 86), P.TWO, PSET
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 230, 62, Z, Y
  NEXT Y
  FOR Y = 0 TO 4
   IF Y = 2 OR Y = 3 THEN Z = 2 ELSE Z = 3
   G1Ellipse 212, 76, Z, Y
  NEXT Y
 GET (399, 53)-STEP(86, 32), P.FOUR

 BFont 1
 FOR X = 1 TO 5
  LINE (510, (X * 33) - 3)-STEP(20, 10), 1, B
  G1Banner CHR$(64 + X), 267, (X * 33) - 4, 1, 1
 NEXT X

 LINE (514, 33)-STEP(9, 5), 1
 LINE STEP(0, 0)-STEP(10, -15), 1
RETURN

ErrGen:
 ERROR 1
END

PERR:
 FOR X = 1 TO 5
  LOCATE 5 + X, 5: PRINT SPACE$(35)
 NEXT X
 LOCATE 7, 6: PRINT "AN ERROR OF TYPE"; ERR; "HAS OCCURED!"
 LOCATE 9, 14: PRINT "ERL RETURNS:"; ERL
 SLEEP
END
     '  MATRIX INFORMATION
     '
     '  DICE%(5)     CURRENT DICE
     '  SCORE%(18,7)  SCOREBOARD
     '           COLUMNS 1-13 BOARD NUMBER
     '           COLUMN    14 BONUS UPPER
     '           COLUMN    15 TOTAL UPPER
     '           COLUMN    16 TOTAL LOWER
     '           COLUMN    17 GRAND TOTAL
     '           COLUMN    18 DONE/NOT DONE STATUS
     '           ROW     1- 7 PLAYERS SCORE
     '           ROW        N COMPUTERS SCORE (<7 PLAYERS)
     '
     '  NAMES$(7)  PLAYERS NAMES
     '  DLogic%(6,5)   COMPUTERS DICE LOGIC
     '           COLUMN     0 DICE NUMBER BY QTY SEQUENCE
     '           COLUMN     1 QTY/POSITION FOR ACES
     '           COLUMN     2 QTY/POSITION FOR TWOS
     '           COLUMN     3 QTY/POSITION FOR THREES
     '           COLUMN     4 QTY/POSITION FOR FOURS
     '           COLUMN     5 QTY/POSITION FOR FIVES
     '           COLUMN     6 QTY/POSITION FOR SIXES
     '           ROW        0 QUANTITY OF DICE
     '           ROW     1- 5 POSITION OF DICE IN DICE%(5)
     '
     '  SAMPLE DLogic%(Y,X) FOR DICE OF 5,2,4,6,4
     '
     '  Y/X   0  1  2  3  4  5
     '  0     4  6  5  2          IN QTY/VALUE SEQUENCE
     '  1                         NO DIE=1
     '  2     1  2                1 TWO POSITION 2
     '  3                         NO DIE=3
     '  4     2  3  5             2 FOURS POSITIONS 3 AND 5
     '  5     1  1                1 FIVE POSITION 1
     '  6     1  4                1 SIX POSITION 4
     '           +           +
     '  +     +  ++++++++++++++++ SECTION FOR POSITION DATA
     '  +     +------------------ COLUMN INDICATES QTY
     '  +------------------------ INDEX EQUATES TO DIE VALUE
     '
     '  TO RUN INTERNAL CHECKS SECTION - ENTER A -1 FOR
     '  PLAYING BOARD NUMBER OR QTY OF DICE TO ROLL
     '  AGAIN - - DATA WILL NOT BE LOST AND SCREEN
     '  WILL BE RESET WITH CURRENT DICE SHOWN - -
     '  PLAYERS/COMPUTERS SCORE WILL NOT SHOW UNTIL
     '  AFTER THEIR TURN



DEFSNG A-Z
SUB ADDSCORE (CHOICE%, PLAYER%)
    
  DIM DLogic%(6, 5)
   
     FOR J = 1 TO 5
      DLogic%(DICE%(J), 0) = DLogic%(DICE%(J), 0) + 1       'SETS FREQUENCY(QTY)
      DLogic%(DICE%(J), DLogic%(DICE%(J), 0)) = J           'SETS POSITION
     NEXT J
    
     X = 0
     FOR J = 5 TO 1 STEP -1
      FOR M = 6 TO 1 STEP -1
       IF DLogic%(M, 0) = J THEN
	DLogic%(0, X) = M: X = X + 1          'SETS VALUE ORDER
       END IF
      NEXT M
     NEXT J
   

 SELECT CASE CHOICE%                    'CHECK PLAYER BOARD 1-13
   CASE IS < 7
       X = -1
	FOR J = 0 TO 4
	 IF DLogic%(0, J) = CHOICE% THEN X = J
	NEXT J
       IF X <> -1 THEN SCORE%(CHOICE%, PLAYER%) = CHOICE% * DLogic%(CHOICE%, 0)
  
   CASE 7, 8
       IF DLogic%(DLogic%(0, 0), 0) > (CHOICE% - 5) THEN
	FOR J = 1 TO 5
	 SCORE%(CHOICE%, PLAYER%) = SCORE%(CHOICE%, PLAYER%) + DICE%(J)
	NEXT J
       END IF
 
   CASE 9
       IF DLogic%(DLogic%(0, 0), 0) = 3 AND DLogic%(DLogic%(0, 1), 0) = 2 THEN SCORE%(9, PLAYER%) = 25
 
   CASE 10
       FOR X = 0 TO 2
	IF DLogic%(1 + X, 0) > 0 AND DLogic%(2 + X, 0) > 0 AND DLogic%(3 + X, 0) > 0 AND DLogic%(4 + X, 0) > 0 THEN SCORE%(10, PLAYER%) = 30
       NEXT X

   CASE 11
       IF DLogic%(1, 0) AND DLogic%(2, 0) AND DLogic%(3, 0) AND DLogic%(4, 0) AND DLogic%(5, 0) = 1 THEN SCORE%(11, PLAYER%) = 40
       IF DLogic%(2, 0) AND DLogic%(3, 0) AND DLogic%(4, 0) AND DLogic%(5, 0) AND DLogic%(6, 0) = 1 THEN SCORE%(11, PLAYER%) = 40

   CASE 12
       IF DLogic%(DLogic%(0, 0), 0) = 5 THEN
	IF SCORE%(12, PLAYER%) < 50 THEN SCORE%(12, PLAYER%) = 50 ELSE SCORE%(12, PLAYER%) = SCORE%(12, PLAYER%) + 100
       END IF

   CASE 13
       FOR J = 1 TO 5
	SCORE%(CHOICE%, PLAYER%) = SCORE%(CHOICE%, PLAYER%) + DICE%(J)
       NEXT J

END SELECT
    
     PLAY "MBt255MLL64O5CC#DD#EFF#GG#AA#B"
  CALL MouseHide
     LOCATE CHOICE% + 3 + 3 * (CHOICE% \ 7), 4 * (PLAYER% - 1) + 15
     PRINT USING "###"; SCORE%(CHOICE%, PLAYER%)
    
     FOR J = 14 TO 17           'GET SCOREBOARD TOTALS
      SCORE%(J, PLAYER%) = 0
     NEXT J
     FOR J = 1 TO 6
      IF SCORE%(J, PLAYER%) > 0 THEN SCORE%(15, PLAYER%) = SCORE%(15, PLAYER%) + SCORE%(J, PLAYER%)
     NEXT J
     FOR J = 7 TO 13
      IF SCORE%(J, PLAYER%) > 0 THEN SCORE%(16, PLAYER%) = SCORE%(16, PLAYER%) + SCORE%(J, PLAYER%)
     NEXT J
     IF SCORE%(15, PLAYER%) > 62 THEN SCORE%(14, PLAYER%) = 35
     SCORE%(17, PLAYER%) = SCORE%(14, PLAYER%) + SCORE%(15, PLAYER%) + SCORE%(16, PLAYER%)
  
     LOCATE 11, 4 * (PLAYER% - 1) + 15: PRINT USING "###"; SCORE%(15, PLAYER%)
     LOCATE 21, 4 * (PLAYER% - 1) + 15: PRINT USING "###"; SCORE%(16, PLAYER%)
     LOCATE 22, 4 * (PLAYER% - 1) + 15: PRINT USING "###"; SCORE%(14, PLAYER%)
     LOCATE 23, 4 * (PLAYER% - 1) + 15: PRINT USING "###"; SCORE%(17, PLAYER%)
  CALL MouseShow

    IF SCORE%(CHOICE%, PLAYER%) = 0 THEN SCORE%(CHOICE%, PLAYER%) = -1

END SUB

DEFINT A-Z
SUB BUTTON (STRG$, X1%, Y1%, UD%)
CALL MouseHide
 X2% = X1% + LEN(STRG$) * 8 + 3
 Y2% = Y1% + 10
 IF UD% <> 0 THEN UD% = 1

 G1Color 1, 0: G1Box X1% - 3, Y1% - 3, X2% + 3, Y2% + 3, 1
 G1Color 0, 0: BFont 0: G1Banner STRG$, X1% + 2, Y1% + 2, 1, 1
 FOR Z = 1 TO 2
  G1Color 3 * UD%, 0
   G1Box X1% - Z, Y1% - Z, X2% + Z, Y2% + Z, 0
  G1Color 3 * (1 - UD%), 0
   G1Line X2% + Z, Y2% + Z, X2% + Z, Y1% - Z
   G1Line X2% + Z, Y2% + Z, X1% - Z, Y2% + Z
 NEXT Z

CALL MouseShow
END SUB

DEFSNG A-Z
FUNCTION GETKEY%

 DO
  A$ = INKEY$
  IF MouseButton(mousePos, FALSE) THEN GOTO ButtonPress
 LOOP UNTIL LEN(A$)
 
 IF ASC(A$) = 27 THEN END

GETKEY% = ASC(A$)
EXIT FUNCTION

ButtonPress:
 COL% = mousePos.X
 ROW% = mousePos.Y
 'LOCATE 2, 2: PRINT COL%; ROW%

 IF COL% > 255 AND COL% < 265 THEN
  IF ROW% > 30 AND ROW% < 40 THEN GETKEY% = ASC("A"): EXIT FUNCTION
  IF ROW% > 63 AND ROW% < 73 THEN GETKEY% = ASC("B"): EXIT FUNCTION
  IF ROW% > 96 AND ROW% < 106 THEN GETKEY% = ASC("C"): EXIT FUNCTION
  IF ROW% > 129 AND ROW% < 139 THEN GETKEY% = ASC("D"): EXIT FUNCTION
  IF ROW% > 162 AND ROW% < 172 THEN GETKEY% = ASC("E"): EXIT FUNCTION
 END IF

 IF COL% > 293 AND COL% < 319 THEN
  IF ROW% > 63 AND ROW% < 134 THEN
    CALL MouseHide
     G1Color 1, 0
      G1Box 294, 65, 317, 132, 0
      G1Box 293, 64, 318, 133, 0
      G1Line 316, 131, 316, 66
     G1Color 0, 0
      G1Line 294, 65, 294, 133
      G1Line 294, 65, 318, 65
      G1Line 293, 64, 293, 133
      G1Line 293, 64, 318, 64
    CALL MouseShow
     DO WHILE MouseButton(mousePos, FALSE)
     LOOP
     FOR PAUSE = 1 TO 50: NEXT PAUSE
    CALL MouseHide
     G1Color 3, 0
      G1Box 294, 65, 317, 132, 0
      G1Box 293, 64, 318, 133, 0
     G1Color 0, 0
      G1Line 294, 132, 317, 132
      G1Line 293, 133, 318, 133
      G1Line 316, 131, 316, 66
      G1Line 317, 132, 317, 65
      G1Line 318, 133, 318, 64
    CALL MouseShow
   GETKEY% = 13: EXIT FUNCTION
  END IF
 END IF

 IF COL% > 53 AND COL% < 149 THEN
  BN% = (ROW% \ 8) + 46
  IF (BN% > 48 AND BN% < 55) OR (BN% > 57 AND BN% < 65) THEN
   IF BN% > 57 THEN BN% = BN% - 3
   IF BN% > 57 THEN BN% = BN% + 7
   GETKEY% = BN%
  END IF
 END IF

END FUNCTION

