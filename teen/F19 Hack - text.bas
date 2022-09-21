REM                            F19 Roster Hack
REM                       Written by Lumpy 28-7-1994

DECLARE FUNCTION GETKEY$ ()
DECLARE SUB SHOWPERSON (PER%)

TYPE QTYPE
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
 OTHER AS STRING * 22
END TYPE

DIM Q AS QTYPE, RAWDATA$(11, 1)
DIM SHARED TABLE(11, 11) AS DOUBLE, NAMES(11) AS STRING * 22, RANK$(6), STAT$(2)

RANK$(0) = "2nd Lt.    ": RANK$(1) = "1st Lt.    ": RANK$(2) = "Captain    ": RANK$(3) = "Major      "
RANK$(4) = "Lt. Colonel": RANK$(5) = "Colonel    ": RANK$(6) = "B. General "
STAT$(0) = "Active ": STAT$(1) = "Retired": STAT$(2) = "K.I.A. "

GOSUB GETDATA
GOSUB SHOWSCREEN
LET PER% = 1: CALL SHOWPERSON(1)

F% = 1
DO
 ON F% GOSUB NOM, RANK, MEDALS, MISSIONS, TOTAL, BEST, LAST, STATUS, SAVE, XIT
 LET F% = F% + 4 - LEN(A$)
 IF F% = 11 THEN F% = 1
 IF F% = 0 THEN F% = 10
LOOP



NOM:
 LOCATE 23, 3, 0: COLOR 2, 0: PRINT "USE ARROW KEYS TO SELECT PERSON"
 LOCATE 4, 2: COLOR 4: PRINT "NAME:"
 LOCATE 20, 47: COLOR 7: PRINT "EXIT"
 LOCATE 4, 34: COLOR 7: PRINT "RANK:"

 DO
  A$ = GETKEY$

  IF A$ = "UP" OR A$ = "DOWN" THEN
   LOCATE 5 + PER%, 4: PRINT NAMES(PER%)
   PER% = PER% + LEN(A$) - 3
   IF PER% < 1 THEN PER% = 1
   IF PER% > 10 THEN PER% = 10
   CALL SHOWPERSON(PER%)
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


RANK:
 LOCATE 4, 2: COLOR 7: PRINT "NAME:"
 LOCATE 9, 34: COLOR 7: PRINT "MEDALS:"
 LOCATE 4, 34: COLOR 4: PRINT "RANK:"
 LOCATE 23, 3: COLOR 2, 0: PRINT "USE ARROW KEYS TO SELECT RANK       "

 DO
  LOCATE 6, 39, 1: COLOR 7, 0
  A$ = GETKEY$
 
  IF A$ = "UP" OR A$ = "DOWN" THEN LET TABLE(PER%, 0) = TABLE(PER%, 0) + LEN(A$) - 3
  IF TABLE(PER%, 0) < 0 THEN TABLE(PER%, 0) = 0
  IF TABLE(PER%, 0) > 6 THEN TABLE(PER%, 0) = 6
  PRINT RANK$(TABLE(PER%, 0))
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN

MEDALS:
 LOCATE 4, 34: COLOR 7: PRINT "RANK:"
 LOCATE 4, 61: COLOR 7: PRINT "MISSIONS:"
 LOCATE 9, 34: COLOR 4: PRINT "MEDALS:"
 LOCATE 23, 3: COLOR 2, 0: PRINT "USE ARROW KEYS AND SPACE TO SELECT       "
 M% = 1

 DO
  LOCATE 9 + M%, 53, 1: COLOR 7, 0
  A$ = GETKEY$

  IF A$ = "UP" OR A$ = "DOWN" THEN LET M% = M% + LEN(A$) - 3
  IF M% < 1 THEN M% = 1
  IF M% > 6 THEN M% = 6
  IF A$ = " " THEN
   IF TABLE(PER%, M%) = 0 THEN TABLE(PER%, M%) = 1: PRINT "X" ELSE TABLE(PER%, M%) = 0: PRINT " "
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


MISSIONS:
 LOCATE 9, 34: COLOR 7: PRINT "MEDALS:"
 LOCATE 9, 61: COLOR 7: PRINT "TOTAL SCORE:"
 LOCATE 4, 61: COLOR 4: PRINT "MISSIONS:"
 LOCATE 23, 3: COLOR 2, 0: PRINT "TYPE IN NUMBER DESIRED                   "
 LET M% = 0

 DO
  LOCATE 6, 66 + M% MOD 2, 1: COLOR 7, 0
  A$ = GETKEY$

  IF ASC(A$) > 47 AND ASC(A$) < 58 THEN
    IF M% MOD 2 = 0 THEN TABLE(PER%, 7) = VAL(A$) * 10
    IF M% MOD 2 = 1 THEN TABLE(PER%, 7) = TABLE(PER%, 7) + VAL(A$)
    LOCATE 6, 66: PRINT USING "##"; TABLE(PER%, 7): M% = M% + 1
  ELSEIF LEN(A$) < 3 THEN
    M% = 0: TABLE(PER%, 7) = 0: LOCATE 6, 66: PRINT " 0"
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


TOTAL:
 LOCATE 4, 61: COLOR 7: PRINT "MISSIONS:"
 LOCATE 13, 61: COLOR 7: PRINT "BEST SCORE:"
 LOCATE 9, 61: COLOR 4: PRINT "TOTAL SCORE:"
 LET M% = 0

 DO
  LOCATE 11, 66 + M%, 1: COLOR 7, 0
  A$ = GETKEY$

  IF ASC(A$) > 47 AND ASC(A$) < 58 THEN
    IF M% = 0 THEN TABLE(PER%, 8) = VAL(A$) * 10000
    IF M% = 1 THEN TABLE(PER%, 8) = TABLE(PER%, 8) + VAL(A$) * 1000
    IF M% = 2 THEN TABLE(PER%, 8) = TABLE(PER%, 8) + VAL(A$) * 100
    IF M% = 3 THEN TABLE(PER%, 8) = TABLE(PER%, 8) + VAL(A$) * 10
    IF M% = 4 THEN TABLE(PER%, 8) = TABLE(PER%, 8) + VAL(A$)
    IF TABLE(PER%, 8) > 60000 THEN TABLE(PER%, 8) = 60000
    LOCATE 11, 66: PRINT USING "##,###"; TABLE(PER%, 8): M% = (M% + 1) MOD 5
  ELSEIF LEN(A$) < 3 THEN
    M% = 0: TABLE(PER%, 8) = 0: LOCATE 11, 66: PRINT "     0"
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


BEST:
 LOCATE 9, 61: COLOR 7: PRINT "TOTAL SCORE:"
 LOCATE 17, 61: COLOR 7: PRINT "LAST SCORE:"
 LOCATE 13, 61: COLOR 4: PRINT "BEST SCORE:"
 LET M% = 0

 DO
  LOCATE 15, 66 + M%, 1: COLOR 7, 0
  A$ = GETKEY$

  IF ASC(A$) > 47 AND ASC(A$) < 58 THEN
    IF M% = 0 THEN TABLE(PER%, 10) = VAL(A$) * 100
    IF M% = 1 THEN TABLE(PER%, 10) = TABLE(PER%, 10) + VAL(A$) * 10
    IF M% = 2 THEN TABLE(PER%, 10) = TABLE(PER%, 10) + VAL(A$)
    LOCATE 15, 66: PRINT USING "###"; TABLE(PER%, 10): M% = (M% + 1) MOD 3
  ELSEIF LEN(A$) < 3 THEN
    M% = 0: TABLE(PER%, 10) = 0: LOCATE 15, 66: PRINT "  0"
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


LAST:
 LOCATE 13, 61: COLOR 7: PRINT "BEST SCORE:"
 LOCATE 18, 2: COLOR 7: PRINT "STATUS:"
 LOCATE 17, 61: COLOR 4: PRINT "LAST SCORE:"
 LET M% = 0

 DO
  LOCATE 19, 66 + M%, 1: COLOR 7, 0
  A$ = GETKEY$

  IF ASC(A$) > 47 AND ASC(A$) < 58 THEN
    IF M% = 0 THEN TABLE(PER%, 9) = VAL(A$) * 100
    IF M% = 1 THEN TABLE(PER%, 9) = TABLE(PER%, 9) + VAL(A$) * 10
    IF M% = 2 THEN TABLE(PER%, 9) = TABLE(PER%, 9) + VAL(A$)
    LOCATE 19, 66: PRINT USING "###"; TABLE(PER%, 9): M% = (M% + 1) MOD 3
  ELSEIF LEN(A$) < 3 THEN
    M% = 0: TABLE(PER%, 9) = 0: LOCATE 19, 66: PRINT "  0"
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


STATUS:
 LOCATE 17, 61: COLOR 7: PRINT "LAST SCORE:"
 LOCATE 20, 36: COLOR 7: PRINT "SAVE"
 LOCATE 18, 2: COLOR 4: PRINT "STATUS:"
 LOCATE 23, 3: COLOR 2, 0: PRINT "USE ARROW KEYS TO SELECT STATUS       "

 DO
  LOCATE 20, 7, 1: COLOR 7, 0
  A$ = GETKEY$

  IF A$ = "UP" OR A$ = "DOWN" THEN LET TABLE(PER%, 11) = TABLE(PER%, 11) + LEN(A$) - 3
  IF TABLE(PER%, 11) < 0 THEN TABLE(PER%, 11) = 0
  IF TABLE(PER%, 11) > 2 THEN TABLE(PER%, 11) = 2
  PRINT STAT$(TABLE(PER%, 11))
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


SAVE:
 LOCATE 18, 2: COLOR 7: PRINT "STATUS:"
 LOCATE 20, 47: COLOR 7: PRINT "EXIT"
 LOCATE 20, 36: COLOR 4: PRINT "SAVE"
 LOCATE 23, 3: COLOR 2, 0: PRINT "PRESS ENTER TO SAVE FILE       "

 DO
  LOCATE 20, 36, 1
  A$ = GETKEY$
 
  IF A$ = CHR$(13) THEN
    COLOR 30: LOCATE 10, 34: PRINT "ÉÍÍÍÍÍÍÍÍÍÍÍ»"
    LOCATE 11, 34: PRINT "º SAVING... º": LOCATE 12, 34: PRINT "ÈÍÍÍÍÍÍÍÍÍÍÍ¼"
   GOSUB SAVEDATA
    COLOR 7: LOCATE 10, 34: PRINT "   Airman's  "
    LOCATE 11, 34: PRINT "   D.F.C.    ": LOCATE 12, 34: PRINT "   Silver Sta"
  END IF
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN


XIT:
 LOCATE 20, 36: COLOR 7: PRINT "SAVE"
 LOCATE 4, 2: COLOR 7: PRINT "NAME:"
 LOCATE 20, 47: COLOR 4: PRINT "EXIT"
 LOCATE 23, 3: COLOR 2, 0: PRINT "PRESS ENTER TO EXIT            "

 DO
  LOCATE 20, 47, 1
  A$ = GETKEY$
 
  IF A$ = CHR$(13) THEN CLS : END
 LOOP UNTIL A$ = "TAB" OR A$ = "S.TAB"
RETURN



GETDATA:

OPEN "F19Rostr.fil" FOR RANDOM AS #1 LEN = 80
 FOR X = 1 TO 11
  GET #1, X, Q
   LET RAWDATA$(X, 0) = Q.NOM: RAWDATA$(X, 1) = Q.OTHER
   LET NAMES(X) = LEFT$(Q.NOM, INSTR(Q.NOM, CHR$(0)))
   LET TABLE(X, 0) = Q.RANK
   LET TABLE(X, 1) = Q.AM: TABLE(X, 2) = Q.DFC
   LET TABLE(X, 3) = Q.SS: TABLE(X, 4) = Q.AFC
   LET TABLE(X, 5) = Q.PH: TABLE(X, 6) = Q.CMOH
   LET TABLE(X, 7) = Q.MSN
   LET TABLE(X, 8) = Q.TOTAL
   LET TABLE(X, 9) = Q.LAST
   LET TABLE(X, 10) = Q.BEST
   LET TABLE(X - 1, 11) = Q.STATUS
 NEXT X
CLOSE #1
RETURN


SAVEDATA:

OPEN "F19rostr.fil" FOR RANDOM AS #1 LEN = 80
 FOR X = 1 TO 10
   LET Q.NOM = RAWDATA$(X, 0): Q.OTHER = RAWDATA$(X, 1)
   LET Q.NUMBER = X - 1
   LET Q.RANK = TABLE(X, 0)
   LET Q.AM = TABLE(X, 1): Q.DFC = TABLE(X, 2)
   LET Q.SS = TABLE(X, 3): Q.AFC = TABLE(X, 4)
   LET Q.PH = TABLE(X, 5): Q.CMOH = TABLE(X, 6)
   LET Q.MSN = TABLE(X, 7)
   LET Q.TOTAL = TABLE(X, 8)
   LET Q.LAST = TABLE(X, 9)
   LET Q.BEST = TABLE(X, 10)
   LET Q.STATUS = TABLE(X - 1, 11)
   PUT #1, X, Q
 NEXT X
CLOSE #1

TYPE FTYPE
 BIT AS INTEGER
END TYPE
DIM FINAL AS FTYPE
OPEN "F19rostr.fil" FOR RANDOM AS #2 LEN = 2
 LET FINAL.BIT = TABLE(10, 11)
 PUT #2, 401, FINAL
CLOSE #2

RETURN


SHOWSCREEN:

SCREEN 0: CLS
COLOR 2: PRINT
PRINT "            F19 ROSTER HACK                            LUMPY '94              "
COLOR 7: PRINT
PRINT " NAME:                           RANK:                      MISSIONS:         "
PRINT " ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿         ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿            ÚÄÄÄÄ¿         "
PRINT " ³                        ³         ³             ³            ³    ³         "
PRINT " ³                        ³         ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÙ            ÀÄÄÄÄÙ         "
PRINT " ³                        ³                                                   "
PRINT " ³                        ³      MEDALS:                    TOTAL SCORE:      "
PRINT " ³                        ³         Airman's       [ ]         ÚÄÄÄÄÄÄÄÄ¿     "
PRINT " ³                        ³         D.F.C.         [ ]         ³        ³     "
PRINT " ³                        ³         Silver Star    [ ]         ÀÄÄÄÄÄÄÄÄÙ     "
PRINT " ³                        ³         A.F.C.         [ ]      BEST SCORE:       "
PRINT " ³                        ³         Purple Heart   [ ]         ÚÄÄÄÄÄ¿        "
PRINT " ³                        ³         Medal of Honor [ ]         ³     ³        "
PRINT " ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                    ÀÄÄÄÄÄÙ        "
PRINT "                                                            LAST SCORE:       "
PRINT " STATUS:                                                       ÚÄÄÄÄÄ¿        "
PRINT "    ÚÄÄÄÄÄÄÄÄÄ¿                  ÉÍÍÍÍÍÍ»   ÉÍÍÍÍÍÍ»           ³     ³        "
PRINT "    ³         ³                  º SAVE º   º EXIT º           ÀÄÄÄÄÄÙ        "
PRINT "    ÀÄÄÄÄÄÄÄÄÄÙ                  ÈÍÍÍÍÍÍ¼   ÈÍÍÍÍÍÍ¼                          "
COLOR 2: PRINT
PRINT "                                              PRESS TAB TO CHANGE FIELD       "
COLOR 7

FOR X = 1 TO 10
 LOCATE 5 + X, 4: PRINT NAMES(X)
NEXT X

RETURN
END

FUNCTION GETKEY$

DO
 A$ = INKEY$
LOOP WHILE A$ = ""

A$ = RIGHT$(A$, 1)
IF A$ = CHR$(27) THEN CLS : END
IF A$ = CHR$(80) THEN A$ = "DOWN"
IF A$ = CHR$(72) THEN A$ = "UP"
IF A$ = CHR$(9) THEN A$ = "TAB"
IF A$ = CHR$(15) THEN A$ = "S.TAB"

GETKEY$ = A$
END FUNCTION

SUB SHOWPERSON (PER%)

LOCATE 5 + PER%, 4: COLOR 0, 7: PRINT NAMES(PER%)

LOCATE 6, 39: COLOR 7, 0: PRINT RANK$(TABLE(PER%, 0))

FOR X = 1 TO 6
 IF TABLE(PER%, X) <> 0 THEN LOCATE 9 + X, 53: PRINT "X" ELSE LOCATE 9 + X, 53: PRINT " "
NEXT X

LOCATE 6, 66: PRINT USING "##"; TABLE(PER%, 7)
LOCATE 11, 66: PRINT USING "##,###"; TABLE(PER%, 8)
LOCATE 15, 66: PRINT USING "###"; TABLE(PER%, 10)
LOCATE 19, 66: PRINT USING "###"; TABLE(PER%, 9)

LOCATE 20, 7: PRINT STAT$(TABLE(PER%, 11))

END SUB

