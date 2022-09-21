
'         °°°±±±²²²ÛÛÛ    F19 Roster Hack  - V.2.1    ÛÛÛ²²²±±±°°°
'         °°°±±±²²²ÛÛÛ   Written by Lumpy 15-5-1995   ÛÛÛ²²²±±±°°°

 DECLARE FUNCTION GETKEY$ ()
 DECLARE SUB BUTTON (STRG$, X1%, Y1%, UD%)
 DECLARE SUB NEWNAME ()
 DECLARE SUB SELECT.MOD (NEWM%)
 DECLARE SUB MOD.NAME ()
 DECLARE SUB MOD.EXIT (MS%)
 DECLARE SUB MOD.RANK ()
 DECLARE SUB MOD.SAVE (MS%)
 DECLARE SUB MOD.SCORES (MS%)
 DECLARE SUB MOD.MEDALS ()
 DECLARE SUB MOD.STATUS ()
 DECLARE SUB MOD.RANK.B ()
 DECLARE SUB MOD.STATUS.B ()
 DECLARE SUB SMOD.SCORE (MODL%)
 DECLARE SUB SMOD.OK (MS%)

' $INCLUDE: 'lumpy2.bi'

TYPE INFO
 STATUS AS INTEGER
 NUMBER AS INTEGER
 NOM AS STRING * 30
 RANK AS INTEGER
 PH AS INTEGER
 AM AS INTEGER
 DFC AS INTEGER
 SS AS INTEGER
 AFC AS INTEGER
 CMOH AS INTEGER
 BEST AS INTEGER
 LAST AS INTEGER
 TOTAL AS LONG
 MSN AS INTEGER
 MISC1 AS STRING * 22
END TYPE

DIM SHARED P(11) AS INFO, WIN.OPEN%, PER%, RET.CODE$, ROW%, COL%, LBP%
DIM SHARED MEDALS(11, 5) AS INTEGER, RANK$(6), STAT$(2), BACK%(3175)
DIM SHARED mousePos AS MouseEvent

RANK$(0) = "2nd Lt.    ": RANK$(1) = "1st Lt.    ": RANK$(2) = "Captain    ": RANK$(3) = "Major      "
RANK$(4) = "Lt. Colonel": RANK$(5) = "Colonel    ": RANK$(6) = "B. General "
STAT$(0) = "Active ": STAT$(1) = "Retired": STAT$(2) = "K.I.A. "

GOSUB SHOWSCREEN
GOSUB GETDATA

 DEF SEG = 0: POKE &H417, 0              'capslock off
 is.mouse% = MouseInit(1)
 MouseGraphicsPtr 5, 0, MapHand

 PER% = 1: RET.CODE$ = "NAME"

DO
  IF RET.CODE$ = "NAME" THEN CALL MOD.NAME
  IF RET.CODE$ = "RANK" THEN CALL MOD.RANK
  IF RET.CODE$ = "RANK.B" THEN CALL MOD.RANK.B
  IF RET.CODE$ = "MEDALS" THEN CALL MOD.MEDALS
  IF RET.CODE$ = "STATUS" THEN CALL MOD.STATUS
  IF RET.CODE$ = "STATUS.B" THEN CALL MOD.STATUS.B
  IF RET.CODE$ = "SCORES" THEN CALL MOD.SCORES(0)
  IF RET.CODE$ = "EXIT" THEN CALL MOD.EXIT(0)
  IF RET.CODE$ = "SAVE" THEN CALL MOD.SAVE(0)
LOOP WHILE LEN(RET.CODE$) > 3

PRINT "OOPS!...How did that happen?!?"
END





GETDATA:
  ERASE BACK%
  GET (85, 85)-(225, 127), BACK%
  G1Color 2, 0: G1Box 90, 90, 225, 127, 1
  G1Color 1, 0: G1Box 85, 85, 220, 122, 1
  G1Color 0, 0: G1Box 87, 87, 218, 120, 0
  BFont 2: G1Banner "READING FILE...", 95, 93, 1, 1
  G1Color 2, 0

 OPEN "F19rostr.fil" FOR RANDOM AS #1 LEN = 80
  FOR X = 1 TO 11
   GET #1, X, P(X)
    P(X).NOM = LEFT$(P(X).NOM, INSTR(P(X).NOM, CHR$(0)) - 1)
    MEDALS(X, 0) = P(X).AM: MEDALS(X, 1) = P(X).DFC: MEDALS(X, 2) = P(X).SS
    MEDALS(X, 3) = P(X).AFC: MEDALS(X, 4) = P(X).PH: MEDALS(X, 5) = P(X).CMOH
   G1Box 93, 111, 93 + X * 11, 115, 1
  NEXT X
 CLOSE #1

 PUT (85, 85), BACK%, PSET
 G1Color 7, 0
 FOR X = 1 TO 10
  LOCATE 5 + X, 2: G1Write LEFT$(P(X).NOM, 22)
 NEXT X

RETURN


SHOWSCREEN:

 SCREEN 1
 G1Mode 1
 G1PaletteB 6

 BFont 2: G1Color 2, 0
 G1Banner "F19", 10, 0, 3, 1
 G1Banner "LUMPY '94", 200, 1, 1, 1
 BFont 1: G1Banner "ROSTER", 85, 2, 1, 1

 G1Color 1, 0
 G1Banner "Name:", 2, 23, 1, 1
 G1Box 2, 35, 189, 124, 0

 G1Banner "Rank:", 195, 23, 1, 1
 G1Box 195, 35, 308, 51, 0
 CALL BUTTON(CHR$(31), 293, 38, 1)

 G1Color 1, 0: BFont 1
 G1Banner "Medals:", 195, 58, 1, 1
  FOR X = 0 TO 5
   G1Ellipse 303, 75 + 8 * X, 3, 3
  NEXT X
 G1Color 3, 0
 LOCATE 10, 26: G1Write "Airman's"
 LOCATE 11, 26: G1Write "D.F.C."
 LOCATE 12, 26: G1Write "Silver Star"
 LOCATE 13, 26: G1Write "A.F.C"
 LOCATE 14, 26: G1Write "Purple Heart"
 LOCATE 15, 26: G1Write "M. of Honor"

 G1Color 1, 0
 G1Banner "Status:", 2, 126, 1, 1
 G1Box 2, 139, 84, 155, 0
 CALL BUTTON(CHR$(31), 70, 142, 1)

 CALL BUTTON(" EXIT ", 250, 145, 1)
 CALL BUTTON(" SAVE ", 250, 165, 1)
 CALL BUTTON(" SCORES... ", 110, 150, 1)

RETURN

DEFSNG A-Z
SUB BUTTON (STRG$, X1%, Y1%, UD%)
CALL MouseHide
 X2% = X1% + LEN(STRG$) * 8 + 3
 Y2% = Y1% + 10
 IF UD% <> 0 THEN UD% = 1

 G1Color 1, 0: G1Box X1%, Y1%, X2%, Y2%, 1
 G1Color 0, 0: BFont 0: G1Banner STRG$, X1% + 2, Y1% + 2, 1, 1
 FOR Z = 1 TO 2
  G1Color 2 + UD%, 0
   G1Box X1% - Z, Y1% - Z, X2% + Z, Y2% + Z, 0
  G1Color 3 - UD%, 0
   G1Line X2% + Z, Y2% + Z, X2% + Z, Y1% - Z
   G1Line X2% + Z, Y2% + Z, X1% - Z, Y2% + Z
 NEXT Z

CALL MouseShow
END SUB

FUNCTION GETKEY$

DO
 A$ = INKEY$
 IF MouseButton(mousePos, FALSE) THEN GOTO ButtonPress
LOOP UNTIL LEN(A$)

A$ = RIGHT$(A$, 1)
IF WIN.OPEN% = 0 THEN
  IF A$ = CHR$(27) THEN CALL MOD.EXIT(1)
  IF A$ = CHR$(80) THEN A$ = "DOWN"
  IF A$ = CHR$(72) THEN A$ = "UP"
END IF
IF A$ = CHR$(27) AND WIN.OPEN% = 1 THEN CALL SMOD.OK(1)

GETKEY$ = A$
EXIT FUNCTION

ButtonPress:
 GETKEY$ = "BP"
 COL% = mousePos.X
 ROW% = mousePos.Y
 'LOCATE 1, 1: PRINT COL%; ROW%

IF WIN.OPEN% = 0 THEN
 IF COL% > 248 AND COL% < 303 AND ROW% > 144 AND ROW% < 157 THEN CALL MOD.EXIT(1)
 IF COL% > 196 AND COL% < 289 AND ROW% > 24 AND ROW% < 50 THEN
   RET.CODE$ = "RANK": EXIT FUNCTION
 END IF
 IF COL% > 196 AND COL% < 305 AND ROW% > 59 AND ROW% < 117 THEN
   RET.CODE$ = "MEDALS": EXIT FUNCTION
 END IF
 IF COL% > 3 AND COL% < 66 AND ROW% > 128 AND ROW% < 155 THEN
   RET.CODE$ = "STATUS": EXIT FUNCTION
 END IF
 IF COL% > 248 AND COL% < 303 AND ROW% > 164 AND ROW% < 177 THEN
   RET.CODE$ = "SAVE": CALL MOD.SAVE(1): EXIT FUNCTION
 END IF
 IF COL% > 108 AND COL% < 203 AND ROW% > 149 AND ROW% < 162 THEN
   CALL MOD.SCORES(1): EXIT FUNCTION
 END IF
 IF COL% > 292 AND COL% < 306 AND ROW% > 37 AND ROW% < 49 THEN
   RET.CODE$ = "RANK.B": EXIT FUNCTION
 END IF
 IF COL% > 69 AND COL% < 83 AND ROW% > 140 AND ROW% < 153 THEN
   RET.CODE$ = "STATUS.B": EXIT FUNCTION
 END IF
 IF COL% > 6 AND COL% < 375 AND ROW% > 25 AND ROW% < 120 THEN
   RET.CODE$ = "NAME": EXIT FUNCTION
 END IF
END IF

IF WIN.OPEN% = 1 THEN
 IF COL% > 148 AND COL% < 198 AND ROW% > 154 AND ROW% < 164 THEN
   CALL SMOD.OK(1): EXIT FUNCTION
 END IF
 IF COL% > 62 AND COL% < 164 AND ROW% > 52 AND ROW% < 66 THEN
   RET.CODE$ = "MISSIONS": EXIT FUNCTION
 END IF
 IF COL% > 84 AND COL% < 190 AND ROW% > 76 AND ROW% < 90 THEN
   RET.CODE$ = "TOTAL": EXIT FUNCTION
 END IF
 IF COL% > 92 AND COL% < 173 AND ROW% > 100 AND ROW% < 114 THEN
   RET.CODE$ = "LAST": EXIT FUNCTION
 END IF
 IF COL% > 92 AND COL% < 173 AND ROW% > 124 AND ROW% < 138 THEN
   RET.CODE$ = "BEST": EXIT FUNCTION
 END IF
END IF

IF WIN.OPEN% = 2 THEN
 IF COL% > 55 AND COL% < 246 AND ROW% > 65 AND ROW% < 114 THEN
   RET.CODE$ = "BPress": EXIT FUNCTION
 END IF
END IF
END FUNCTION

SUB MOD.EXIT (MS%)
  RET.CODE$ = ""
  DO
   IF MS% THEN A$ = CHR$(13) ELSE SELECT.MOD (6): A$ = GETKEY$
  
   IF A$ = CHR$(9) THEN RET.CODE$ = "SAVE"
   IF A$ = CHR$(15) THEN RET.CODE$ = "SCORES"
   IF A$ = CHR$(13) THEN
    CALL BUTTON(" EXIT ", 250, 145, 0)
     DO
     LOOP WHILE MouseButton(mousePos, FALSE)
    CALL BUTTON(" EXIT ", 250, 145, 1)
    G1Mode 0: END
   END IF
  LOOP UNTIL LEN(RET.CODE$) > 2
END SUB

DEFINT A-Z
SUB MOD.MEDALS
 RET.CODE$ = ""
  SELECT.MOD (3)
   STATIC M%
 
   IF mousePos.buttons AND ROW% > 70 THEN
    CALL MouseHide
      G1Color 1, 0: G1Ellipse 303, 75 + 8 * M%, 3, 3
      M% = ROW% \ 8 - 9
      IF M% > 5 OR M% < 0 THEN M% = 1
      IF MEDALS(PER%, M%) THEN MEDALS(PER%, M%) = 0 ELSE MEDALS(PER%, M%) = 1
      G1Color MEDALS(PER%, M%), 0: G1Plot 303, 75 + 8 * M%
      G1Ellipse 303, 75 + 8 * M%, 1, 1
   END IF

   DO
     G1Color 2, 0: G1Ellipse 303, 75 + 8 * M%, 3, 3
     CALL MouseShow
    A$ = GETKEY$
     G1Color 1, 0: G1Ellipse 303, 75 + 8 * M%, 3, 3

    IF A$ = CHR$(9) THEN RET.CODE$ = "STATUS"
    IF A$ = CHR$(15) THEN RET.CODE$ = "RANK"
    IF A$ = "UP" OR A$ = "DOWN" THEN LET M% = M% + LEN(A$) - 3
    IF M% < 0 THEN M% = 0
    IF M% > 5 THEN M% = 5
    IF A$ = " " THEN
     IF MEDALS(PER%, M%) THEN MEDALS(PER%, M%) = 0 ELSE MEDALS(PER%, M%) = 1
     G1Color MEDALS(PER%, M%), 0: G1Plot 303, 75 + 8 * M%: G1Ellipse 303, 75 + 8 * M%, 1, 1
    END IF
   LOOP UNTIL LEN(RET.CODE$) > 2

END SUB

SUB MOD.NAME
   RET.CODE$ = ""
   SELECT.MOD (1)

   IF mousePos.buttons = LEFT AND ROW% > 40 THEN
    CALL MouseHide
    G1Color 3, 0: LOCATE 5 + PER%, 2: G1Write LEFT$(P(PER%).NOM, 22)
    PER% = ROW% \ 8 - 4
    IF PER% < 0 OR PER% > 10 THEN PER% = 1
    FOR PAUSE = 1 TO 2000: NEXT PAUSE
    IF MouseButton(mousePos, FALSE) THEN A$ = CHR$(13)
   END IF

   DO
    G1Color 3, 0

    IF A$ = "UP" OR A$ = "DOWN" THEN
     LOCATE 5 + PER%, 2: G1Write LEFT$(P(PER%).NOM, 22)
     PER% = (PER% + LEN(A$) - 3) MOD 10
     IF PER% < 1 THEN PER% = 10
    END IF
      LOCATE 5 + PER%, 2: G1Color 0, 3: G1Write LEFT$(P(PER%).NOM, 22)
      LOCATE 6, 26: G1Color 3, 0: G1Write RANK$(P(PER%).RANK)
      G1Color 1, 0
      FOR X = 0 TO 5
       IF MEDALS(PER%, X) THEN G1Color 1, 0 ELSE G1Color 0, 0
       G1Plot 303, 75 + 8 * X: G1Ellipse 303, 75 + 8 * X, 1, 1
      NEXT X
      LOCATE 19, 2: G1Color 3, 0: G1Write STAT$(P(PER% + 1).STATUS)
      CALL MouseShow
    IF A$ = CHR$(13) THEN CALL NEWNAME
   
    A$ = GETKEY$
    IF A$ = CHR$(9) THEN RET.CODE$ = "RANK"
    IF A$ = CHR$(15) THEN RET.CODE$ = "SAVE"
   LOOP UNTIL LEN(RET.CODE$) > 2

END SUB

SUB MOD.RANK
   RET.CODE$ = ""
   SELECT.MOD (2)

   DO
    A$ = GETKEY$
    LOCATE 6, 26, 1: G1Color 3, 0

    IF A$ = CHR$(9) THEN RET.CODE$ = "MEDALS"
    IF A$ = CHR$(15) THEN RET.CODE$ = "NAME"
    IF A$ = "UP" OR A$ = "DOWN" THEN
     P(PER%).RANK = P(PER%).RANK + LEN(A$) - 3
     IF P(PER%).RANK < 0 THEN P(PER%).RANK = 0
     IF P(PER%).RANK > 6 THEN P(PER%).RANK = 6
    END IF
    G1Write RANK$(P(PER%).RANK)
   LOOP UNTIL LEN(RET.CODE$) > 2

END SUB

SUB MOD.RANK.B
   CALL BUTTON(CHR$(31), 293, 38, 0)
    DO
    LOOP WHILE MouseButton(mousePos, FALSE)
   CALL BUTTON(CHR$(31), 293, 38, 1)
    SELECT.MOD (2)
    CALL MouseHide
     ERASE BACK%: GET (200, 51)-(308, 115), BACK
     LINE (200, 51)-(308, 115), 0, BF: LINE (200, 51)-(308, 115), 1, B
    CALL MouseShow
    FOR X = 0 TO 6
     LOCATE 8 + X, 27: PRINT RANK$(X)
    NEXT X
    P = 1
    G1Color 0, 3
     DO
       CALL MouseGetInfo(mousePos)
       IF mousePos.Y > 55 AND mousePos.Y < 110 THEN
	OP = P: P = mousePos.Y \ 8 - 7
	IF OP - P THEN
	 LOCATE 8 + OP, 27: PRINT RANK$(OP)
	 LOCATE 8 + P, 27: G1Write RANK$(P)
	END IF
       END IF
     LOOP UNTIL MouseButton(mousePos, FALSE)
    CALL MouseHide
     PUT (200, 51), BACK, PSET
    CALL MouseShow
    LOCATE 6, 26: PRINT RANK$(P)
    P(PER%).RANK = P
    RET.CODE$ = "RANK"
END SUB

SUB MOD.SAVE (MS%)
  IF MS% = 0 THEN SELECT.MOD (7): RET.CODE$ = ""
  DO
   IF A$ = CHR$(13) OR MS% THEN
    CALL BUTTON(" SAVE ", 250, 165, 0)
    DO
    LOOP WHILE MouseButton(mousePos, FALSE)
    CALL BUTTON(" SAVE ", 250, 165, 1)
    
     ERASE BACK%
     GET (105, 85)-(200, 127), BACK%
     G1Color 2, 0: G1Box 110, 90, 200, 127, 1
     G1Color 1, 0: G1Box 105, 85, 195, 122, 1
     G1Color 0, 0: G1Box 107, 87, 193, 120, 0
     BFont 2: G1Banner "SAVING...", 115, 93, 1, 1
     G1Color 2, 0
    
    OPEN "F19rostr.fil" FOR RANDOM AS #1 LEN = 80
     FOR X = 1 TO 10
       P(X).AM = MEDALS(X, 0): P(X).DFC = MEDALS(X, 1): P(X).SS = MEDALS(X, 2)
       P(X).AFC = MEDALS(X, 3): P(X).PH = MEDALS(X, 4): P(X).CMOH = MEDALS(X, 5)
      PUT #1, X, P(X)
      G1Box 115, 111, 115 + X * 7, 115, 1
     NEXT X
    CLOSE #1
    OPEN "F19rostr.fil" FOR BINARY AS #1
     PUT #1, 801, P(11).STATUS
    CLOSE #1

     PUT (105, 85), BACK%, PSET
     IF MS% THEN EXIT SUB
   END IF
 
  A$ = GETKEY$
   IF A$ = CHR$(9) THEN RET.CODE$ = "NAME"
   IF A$ = CHR$(15) THEN RET.CODE$ = "EXIT"
  LOOP UNTIL LEN(RET.CODE$) > 2
END SUB

SUB MOD.SCORES (MS%)
  IF MS% = 0 THEN SELECT.MOD (5): RET.CODE$ = ""
  DO
   IF A$ = CHR$(13) OR MS% THEN
      CALL BUTTON(" SCORES... ", 110, 150, 0)
      DO
      LOOP WHILE MouseButton(mousePos, FALSE)
      CALL BUTTON(" SCORES... ", 110, 150, 1)
  
	CALL MouseHide
	WIN.OPEN% = 1
	ERASE BACK%: GET (50, 30)-(215, 180), BACK%

	BFont 1
	G1Color 2, 0: G1Box 55, 35, 215, 180, 1
	G1Color 1, 0: G1Box 50, 30, 210, 175, 1
	G1Color 2, 0: G1Banner "Scores:", 55, 30, 2, 1
	G1Color 0, 0: G1Box 52, 43, 208, 173, 0
	 G1Box 136, 51, 165, 67, 0
	 G1Box 136, 75, 190, 91, 0
	 G1Box 136, 99, 174, 115, 0
	 G1Box 136, 123, 174, 139, 0
	G1Banner "Missions:", 60, 52, 1, 1
	G1Banner "Total:", 83, 76, 1, 1
	G1Banner "Last:", 91, 100, 1, 1
	G1Banner "Best:", 91, 124, 1, 1
	CALL BUTTON("  OK  ", 147, 154, 1)
	G1Color 3, 1
	 LOCATE 8, 19: G1Write MID$(STR$(P(PER%).MSN), 2)
	 LOCATE 11, 19: G1Write MID$(STR$(P(PER%).TOTAL), 2)
	 LOCATE 14, 19: G1Write MID$(STR$(P(PER%).LAST), 2)
	 LOCATE 17, 19: G1Write MID$(STR$(P(PER%).BEST), 2)
	CALL MouseShow

	 RET.CODE$ = "MISSIONS"
	 DO
	  IF RET.CODE$ = "OK" THEN CALL SMOD.OK(0)
	  IF RET.CODE$ = "MISSIONS" THEN CALL SMOD.SCORE(1)
	  IF RET.CODE$ = "TOTAL" THEN CALL SMOD.SCORE(2)
	  IF RET.CODE$ = "LAST" THEN CALL SMOD.SCORE(3)
	  IF RET.CODE$ = "BEST" THEN CALL SMOD.SCORE(4)
	 LOOP WHILE WIN.OPEN%
	
	RET.CODE$ = "SCORES": EXIT SUB
   END IF

   A$ = GETKEY$
   IF A$ = CHR$(9) THEN RET.CODE$ = "EXIT"
   IF A$ = CHR$(15) THEN RET.CODE$ = "STATUS"
  LOOP UNTIL LEN(RET.CODE$) > 2
END SUB

SUB MOD.STATUS
   RET.CODE$ = ""
   SELECT.MOD (4)
  
   DO
    A$ = GETKEY$
    LOCATE 19, 2, 1: G1Color 3, 0

    IF A$ = CHR$(9) THEN RET.CODE$ = "SCORES"
    IF A$ = CHR$(15) THEN RET.CODE$ = "MEDALS"
    IF A$ = "UP" OR A$ = "DOWN" THEN
     P(PER% + 1).STATUS = P(PER% + 1).STATUS + LEN(A$) - 3
     IF P(PER% + 1).STATUS < 0 THEN P(PER% + 1).STATUS = 0
     IF P(PER% + 1).STATUS > 2 THEN P(PER% + 1).STATUS = 2
    END IF
    G1Write STAT$(P(PER% + 1).STATUS)
   LOOP UNTIL LEN(RET.CODE$) > 2

END SUB

SUB MOD.STATUS.B
   CALL BUTTON(CHR$(31), 70, 142, 0)
    DO
    LOOP WHILE MouseButton(mousePos, FALSE)
   CALL BUTTON(CHR$(31), 70, 142, 1)
    SELECT.MOD (4)
    CALL MouseHide
     ERASE BACK%: GET (10, 155)-(84, 187), BACK
     LINE (10, 155)-(84, 187), 0, BF: LINE (10, 155)-(84, 187), 1, B
    CALL MouseShow
    FOR X = 0 TO 2
     LOCATE 21 + X, 3: PRINT STAT$(X)
    NEXT X
    P = 1
    G1Color 0, 3
     DO
       CALL MouseGetInfo(mousePos)
       IF mousePos.Y > 160 AND mousePos.Y < 185 THEN
	OP = P: P = mousePos.Y \ 8 - 20
	IF OP - P THEN
	 LOCATE 21 + OP, 3: PRINT STAT$(OP)
	 LOCATE 21 + P, 3: G1Write STAT$(P)
	END IF
       END IF
     LOOP UNTIL MouseButton(mousePos, FALSE)
    CALL MouseHide
     PUT (10, 155), BACK, PSET
    CALL MouseShow
    LOCATE 19, 2: PRINT STAT$(P)
    P(PER% + 1).STATUS = P
    RET.CODE$ = "STATUS"
END SUB

DEFSNG A-Z
SUB NEWNAME
 RET.CODE$ = "": WIN.OPEN% = 2
 CALL MouseHide
 ERASE BACK%: GET (55, 65)-(252, 120), BACK%
   
 G1Color 2, 0: G1Box 60, 70, 252, 120, 1
 G1Color 1, 0: G1Box 55, 65, 247, 115, 1
 G1Color 0, 0: G1Box 57, 80, 245, 113, 0
 G1Color 0, 0: G1Box 61, 94, 242, 105, 1
 G1Color 2, 0: BFont 1: G1Banner "Enter new name:", 61, 66, 1, 1
 CALL MouseShow
  
 NAME$ = "": A$ = "": X = 0
 DO
  DO
   A$ = GETKEY$

   IF A$ = CHR$(27) OR LEN(RET.CODE$) THEN GOTO CLOSE.WIN
   IF A$ = CHR$(8) AND X THEN X = X - 1: LOCATE 13, 9 + X: PRINT " "; : NAME$ = LEFT$(NAME$, X)
  LOOP WHILE LEN(A$) > 1 OR ASC(A$) = 8
  LOCATE 13, 9 + X: PRINT A$;
  X = X + 1: NAME$ = NAME$ + A$
 LOOP UNTIL A$ = CHR$(13) OR X = 22
  
 MID$(NAME$, X) = CHR$(0)
 P(PER%).NOM = NAME$
CLOSE.WIN:
 CALL MouseHide
  PUT (55, 65), BACK%, PSET
 CALL MouseShow
 WIN.OPEN% = 0
 LOCATE 5 + PER%, 2: G1Color 0, 3
 G1Write LEFT$(P(PER%).NOM, 22)
 G1Color 3, 0

END SUB

SUB SELECT.MOD (NEWM%)
 STATIC OLDM%, OLDM.S%

 BFont 1
 IF WIN.OPEN% THEN GOTO ScoreMods
 IF NEWM% = OLDM% THEN EXIT SUB
 CALL MouseHide

 FOR C% = 1 TO 2
  IF C% = 2 THEN M% = NEWM% ELSE M% = OLDM%
  SELECT CASE M%
   CASE 1
    G1Color C%, 0: G1Banner "Name:", 2, 23, 1, 1
   CASE 2
    G1Color C%, 0: G1Banner "Rank:", 195, 23, 1, 1
   CASE 3
    G1Color C%, 0: G1Banner "Medals:", 195, 58, 1, 1
   CASE 4
    G1Color C%, 0: G1Banner "Status:", 2, 126, 1, 1
   CASE 5
    G1Color C% - 1, 0: G1Box 106, 146, 205, 164, 0
   CASE 6
    G1Color C% - 1, 0: G1Box 245, 141, 306, 159, 0
   CASE 7
    G1Color C% - 1, 0: G1Box 245, 161, 306, 179, 0
   CASE ELSE
  END SELECT
 NEXT C%

 OLDM% = NEWM%

CALL MouseShow
EXIT SUB

ScoreMods:
 CALL MouseHide

 FOR C% = 0 TO 2 STEP 2
  IF C% = 2 THEN M% = NEWM% ELSE M% = OLDM.S%
  SELECT CASE M%
   CASE 1
    G1Color C%, 0: G1Banner "Missions:", 60, 52, 1, 1
    LINE (144, 64)-STEP(14, 0), 1
   CASE 2
    G1Color C%, 0: G1Banner "Total:", 83, 76, 1, 1
    LINE (144, 88)-STEP(42, 0), 1
   CASE 3
    G1Color C%, 0: G1Banner "Last:", 91, 100, 1, 1
    LINE (144, 112)-STEP(26, 0), 1
   CASE 4
    G1Color C%, 0: G1Banner "Best:", 91, 124, 1, 1
    LINE (144, 136)-STEP(26, 0), 1
   CASE 5
    G1Color 1 - C% \ 2, 0
    G1Box 143, 150, 202, 168, 0
   CASE ELSE
  END SELECT
 NEXT C%

 OLDM.S% = NEWM%

CALL MouseShow
END SUB

SUB SMOD.OK (MS%)
  RET.CODE$ = ""
  DO
   IF MS% THEN A$ = CHR$(13) ELSE SELECT.MOD (5): A$ = GETKEY$
   IF WIN.OPEN% = 0 THEN EXIT SUB
   
   IF A$ = CHR$(9) THEN RET.CODE$ = "MISSIONS"
   IF A$ = CHR$(15) THEN RET.CODE$ = "BEST"
   IF A$ = CHR$(13) THEN
    CALL BUTTON("  OK  ", 147, 154, 0)
    DO
    LOOP WHILE MouseButton(mousePos, FALSE)
    CALL BUTTON("  OK  ", 147, 154, 1)
    CALL MouseHide
     PUT (50, 30), BACK%, PSET
    CALL MouseShow
    RET.CODE$ = "SCORES": WIN.OPEN% = 0
    EXIT SUB
   END IF

  LOOP UNTIL LEN(RET.CODE$) > 2
END SUB

DEFINT A-Z
SUB SMOD.SCORE (MODL%)
 RET.CODE$ = ""
 SELECT.MOD (MODL%)
 IF MODL% = 1 OR MODL% = 2 THEN LIM% = 3 * MODL% - 1 ELSE LIM% = 3
 X% = 5 + 3 * MODL%
 
	NUM$ = "N"
	G1Color 3, 1
	DO
	 LINE ((18 + Z%) * 8, X% * 8)-STEP(6, 0), 0
	 A$ = GETKEY$
	  IF ASC(A$) > 47 AND ASC(A$) < 58 THEN
	    IF Z% = 0 THEN NUM$ = STRING$(LIM%, 48)
	    MID$(NUM$, Z% + 1) = A$
	    LOCATE X%, 19: G1Write NUM$
	    LINE ((18 + Z%) * 8, X% * 8)-STEP(6, 0), 1
	    Z% = (Z% + 1) MOD LIM%
	  END IF
	LOOP UNTIL LEN(RET.CODE$) > 2 OR ASC(A$) = 9 OR ASC(A$) = 15
 
  SELECT CASE MODL%
  CASE 1
   IF NUM$ <> "N" THEN P(PER%).MSN = VAL(NUM$)
   IF A$ = CHR$(9) THEN RET.CODE$ = "TOTAL"
   IF A$ = CHR$(15) THEN RET.CODE$ = "OK"
  CASE 2
   IF NUM$ <> "N" THEN P(PER%).TOTAL = VAL(NUM$)
   IF A$ = CHR$(9) THEN RET.CODE$ = "LAST"
   IF A$ = CHR$(15) THEN RET.CODE$ = "MISSIONS"
  CASE 3
   IF NUM$ <> "N" THEN P(PER%).LAST = VAL(NUM$)
   IF A$ = CHR$(9) THEN RET.CODE$ = "BEST"
   IF A$ = CHR$(15) THEN RET.CODE$ = "TOTAL"
  CASE 4
   IF NUM$ <> "N" THEN P(PER%).BEST = VAL(NUM$)
   IF A$ = CHR$(9) THEN RET.CODE$ = "OK"
   IF A$ = CHR$(15) THEN RET.CODE$ = "LAST"
  END SELECT
 
END SUB

