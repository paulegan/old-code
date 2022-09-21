

'              ***********        DOORS        ***********

'                            PAUL EGAN 1993


DECLARE SUB DOC ()
DECLARE SUB MENU ()
KEY 15, CHR$(0) + CHR$(56)
 ON ERROR GOTO PERR
 ON KEY(8) GOSUB VEND: KEY(8) ON
 ON KEY(9) GOSUB SEND: KEY(9) ON
 ON KEY(10) GOSUB PEND: KEY(10) ON
 ON KEY(15) GOSUB MENU: KEY(15) ON
 DIM SHARED NAME$(14), PATH$(14), FILE$(14), NL(14)
 DIM SHARED P, DFILE$
 DFILE$ = "LIST1.DAT"
VSTART:
 DEF SEG = 0: POKE &H417, 0
' CHDIR "C:\EVERY\TOOLS\MENU"
 COLOR 2, 0: CLS
 SCREEN 0: WIDTH 40
 GOTO GETINFO
START:

LOCATE 1, 1: COLOR 0, 3: PRINT " ABOUT  CHANGE  DOS CMNDS    HELP  QUIT "
COLOR 4, 3: LOCATE 1, 2: PRINT "A": LOCATE 1, 9: PRINT "C"
LOCATE 1, 17: PRINT "D": LOCATE 1, 30: PRINT "H": LOCATE 1, 36: PRINT "Q"

FOR X = 1 TO 14 STEP 2
 LOCATE X + 5, 3: COLOR NL(X), 0: PRINT NAME$(X)
 LOCATE X + 5, 23: COLOR NL(X + 1), 0: PRINT NAME$(X + 1)
NEXT X
LOCATE 6, 3: COLOR NL(1), 7: PRINT NAME$(1)

LOCATE 23, 1: COLOR 14, 3: PRINT "\" + LEFT$(DFILE$, 5)
LOCATE 23, 7: COLOR 0, 3: PRINT "  LEFT MOUSE BUTTON TO SELECT    "
P = 1
IF CHNG = 1 THEN RETURN

GETLOOP:
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
RLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO WHICH
GOTO RLOOP

WHICH:
 IF CHNG = 1 THEN GOTO CWHICH
 IF X$ = CHR$(0) + "P" THEN GOTO DOWN
 IF X$ = CHR$(0) + "H" THEN GOTO UP
 IF X$ = CHR$(0) + "M" THEN GOTO RIGHT
 IF X$ = CHR$(0) + "K" THEN GOTO LEFT
 IF X$ = CHR$(13) THEN GOTO SLCT
 IF X$ = "M" OR X$ = "m" THEN GOTO MENU
 IF X$ = CHR$(27) THEN GOTO L1RET
 IF X$ = "C" OR X$ = "c" THEN GOTO FCHANGE
GOTO GETLOOP
END

SLCT:
COLOR 7, 0: CLS : WIDTH 80
 LST = NL(P): SPATH$ = PATH$(P): SFILE$ = FILE$(P)
IF LST = 5 THEN GOTO IFLIST

BLST$ = "menu.BAT " + DFILE$
BPATH$ = "CD " + SPATH$
OPEN "DO.BAT" FOR OUTPUT AS #3
 PRINT #3, "@ECHO OFF"
 PRINT #3, BPATH$
 PRINT #3, SFILE$
 PRINT #3, BLST$
CLOSE #3
END


DOWN:
 LET DM = 2
 IF P = 13 OR P = 14 THEN GOTO GETLOOP
 IF NAME$(P + 2) = SPACE$(16) AND CHNG <> 1 THEN
  FOR DM = 4 TO 14 STEP 2
   IF P + DM > 14 THEN GOTO GETLOOP
   IF NAME$(P + DM) <> SPACE$(16) THEN GOTO DIFX
  NEXT DM
 END IF
DIFX:
 PL = P + 5: PR = 3
 IF P MOD 2 = 0 THEN LET PR = 23: PL = PL - 1
 LOCATE PL, PR: COLOR NL(P), 0: PRINT NAME$(P)
 LOCATE PL + DM, PR: COLOR NL(P + DM), 7: PRINT NAME$(P + DM)
 P = P + DM
GOTO GETLOOP

UP:
 LET UM = 2
 IF P = 1 OR P = 2 THEN GOTO GETLOOP
 IF NAME$(P - 2) = SPACE$(16) AND CHNG <> 1 THEN
  FOR UM = 4 TO 14 STEP -2
   IF P - UM < 1 THEN GOTO GETLOOP
   IF NAME$(P - DM) <> SPACE$(16) THEN GOTO UIFX
  NEXT UM
 END IF
UIFX:
 PL = P + 5: PR = 3
 IF P MOD 2 = 0 THEN LET PR = 23: PL = PL - 1
 LOCATE PL, PR: COLOR NL(P), 0: PRINT NAME$(P)
 LOCATE PL - UM, PR: COLOR NL(P - UM), 7: PRINT NAME$(P - UM)
 P = P - UM
GOTO GETLOOP

LEFT:
 IF P MOD 2 > 0 THEN GOTO GETLOOP
 IF NAME$(P - 1) = SPACE$(16) AND CHNG <> 1 THEN GOTO GETLOOP
 LOCATE P + 4, 23: COLOR NL(P), 0: PRINT NAME$(P)
 LOCATE P + 4, 3: COLOR NL(P - 1), 7: PRINT NAME$(P - 1)
 P = P - 1
GOTO GETLOOP

RIGHT:
 IF P MOD 2 = 0 THEN GOTO GETLOOP
 IF NAME$(P + 1) = SPACE$(16) AND CHNG <> 1 THEN GOTO GETLOOP
 LOCATE P + 5, 3: COLOR NL(P), 0: PRINT NAME$(P)
 LOCATE P + 5, 23: COLOR NL(P + 1), 7: PRINT NAME$(P + 1)
 P = P + 1
GOTO GETLOOP
 
PEND:
 OPEN "DO.BAT" FOR OUTPUT AS #3
  PRINT #3, "@ECHO OFF"
 CLOSE #3
 COLOR 7, 0
END

VEND:
 DEF SEG = 0: POKE &H417, 64
 COLOR 7, 0
END

SEND:
 CLS : WIDTH 80: SHELL: GOTO VSTART

PERR:
 IF ERR = 64 THEN GOTO FNF
 PRINT ERR
END

FNF:
COLOR 14, 0
 LOCATE 7, 6: PRINT "������������������������ͻ"
 LOCATE 8, 6: PRINT "�     FILE NOT FOUND     �"
 LOCATE 9, 6: PRINT "�                        �"
 LOCATE 10, 6: PRINT "�   PLEASE MODIFY WITH   �"
 LOCATE 11, 6: PRINT "�    THE CHANGE MENU.    �"
 LOCATE 12, 6: PRINT "������������������������ͼ"
 LOCATE 8, 12: COLOR 9, 0: PRINT "FILE NOT FOUND"
 LOCATE 10, 10: COLOR 6, 0: PRINT "PLEASE MODIFY WITH"
 LOCATE 11, 11: PRINT "THE CHANGE MENU."
DO
LOOP WHILE INKEY$ = ""
FOR X = 12 TO 7 STEP -1
 COLOR 0, 0: LOCATE X, 6: PRINT SPACE$(27)
 IF X MOD 2 = 0 THEN
  IF X - 5 = P THEN RC1 = 7 ELSE RC1 = 0
   LOCATE X, 3: COLOR NL(X - 5), RC1: PRINT NAME$(X - 5)
  IF X - 4 = P THEN RC2 = 7 ELSE RC2 = 0
   LOCATE X, 23: COLOR NL(X - 4), RC2: PRINT NAME$(X - 4)
 END IF
NEXT X
KEY(10) ON: KEY(15) ON: KEY(9) ON
GOTO GETLOOP

IFLIST:
 SLAST$ = LAST$: LAST$ = DFILE$
 IF NL(P) = 5 THEN DFILE$ = LEFT$(NAME$(P), 5) + ".DAT"
GOTO VSTART

L1RET:
 IF LAST$ = "" THEN GOTO GETLOOP ELSE LET DFILE$ = LAST$
 LET LAST$ = SLAST$
GOTO VSTART

MENU:
 MGO = 1
 TYPE GABTYPE
  A AS STRING * 25
  B AS STRING * 25
  C AS STRING * 25
  D AS STRING * 25
  E AS STRING * 12
 END TYPE
 DIM SHARED GAB AS GABTYPE
 DEF SEG = 0: POKE &H417, 64
 CALL MENU
  KEY(15) ON
 DEF SEG = 0: POKE &H417, 0
 IF MGO = 5 THEN
  CALL DOC
  WIDTH 40: GOTO START
 END IF
 ON MGO GOTO GETLOOP, FCHANGE, VSTART
END

FCHANGE:
WIDTH 80
COLOR 7, 0
 LOCATE 3, 41: PRINT "��������������������������������������ͻ"
FOR K = 4 TO 20
 LOCATE K, 41: PRINT CHR$(186)
 LOCATE K, 80: PRINT CHR$(186)
NEXT K
 LOCATE 21, 41: PRINT "��������������������������������������ͼ"
 LOCATE 9, 41: PRINT "��������������������������������������Ķ"
 LOCATE 15, 41: PRINT "��������������������������������������Ķ"
 LOCATE 23, 40: COLOR 9, 3: PRINT SPACE$(41)
 LOCATE 5, 50: COLOR 14, 0: PRINT "SELECT FILE TO CHANGE"
 LOCATE 1, 40: COLOR 0, 3: PRINT SPACE$(41)
CHNG = 1: GOSUB START
GOTO GETLOOP
CWHICH:
 IF X$ = CHR$(0) + "P" THEN GOTO DOWN
 IF X$ = CHR$(0) + "H" THEN GOTO UP
 IF X$ = CHR$(0) + "M" THEN GOTO RIGHT
 IF X$ = CHR$(0) + "K" THEN GOTO LEFT
 IF X$ = CHR$(13) THEN GOTO CSLCT
 IF X$ = CHR$(27) THEN GOTO CEXIT
GOTO GETLOOP
END

CSLCT:
 DEF SEG = 0: POKE &H417, 64
LOCATE 5, 50: COLOR 6, 0: PRINT "SELECT FILE TO CHANGE"
LOCATE 7, 59: COLOR 25, 0: PRINT P
 LOCATE 11, 43: COLOR 1, 0: PRINT "CURRENT NAME:"
 LOCATE 12, 43: COLOR 1, 0: PRINT "        PATH:"
 LOCATE 13, 43: COLOR 1, 0: PRINT "        FILE:"
 LOCATE 11, 57: COLOR 4, 0: PRINT NAME$(P)
 LOCATE 12, 57: COLOR 4, 0: PRINT LEFT$(PATH$(P), 23)
 LOCATE 13, 57: COLOR 4, 0: PRINT FILE$(P)
 LOCATE 17, 43: COLOR 1, 0: PRINT "NEW NAME:"
 LOCATE 18, 43: COLOR 1, 0: PRINT "    PATH:"
 LOCATE 19, 43: COLOR 1, 0: PRINT "    FILE:"
 LOCATE 17, 53: COLOR 4, 0: INPUT ; "", NAME$(P)
 LOCATE 18, 53: COLOR 4, 0: INPUT ; "", PATH$(P)
 LOCATE 19, 53: COLOR 4, 0: INPUT ; "", FILE$(P)
LOCATE 7, 59: COLOR 9, 0: PRINT P
GOSUB CHNGBAT
GOSUB CHNGCOM
TYPE CHNTYPE
 N AS STRING * 16
 P AS STRING * 25
 F AS STRING * 14
END TYPE
DIM CHN AS CHNTYPE
 CHN.N = NAME$(P)
 CHN.P = PATH$(P)
 CHN.F = FILE$(P)
OPEN DFILE$ FOR RANDOM AS #1 LEN = 55
 PUT #1, P, CHN
CLOSE #1
LOCATE 23, 48: COLOR 20, 3: PRINT "  CHANGE ANOTHER FILE?  "
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
ALOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO YESNO
GOTO ALOOP
YESNO:
 IF X$ = "Y" OR X$ = "y" THEN GOTO CCLEAR
GOTO CEXIT

CHNGCOM:
IF NAME$(P) = "" THEN RETURN
LOCATE 23, 49: COLOR 20, 3: PRINT "ENTER COMMENT ON FILE ?"
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
COMLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO COMNO
GOTO COMLOOP
COMNO:
 IF X$ = "N" OR X$ = "n" THEN RETURN
LOCATE 23, 49: COLOR 3, 3: PRINT SPACE$(25)
COLOR 7, 0
 LOCATE 16, 40: PRINT "ɹ": LOCATE 17, 40: PRINT "�"
 LOCATE 18, 40: PRINT "�": LOCATE 19, 40: PRINT "�"
 LOCATE 20, 40: PRINT "�": LOCATE 21, 40: PRINT "��"
FOR X = 39 TO 15 STEP -1
 LOCATE 16, X: PRINT "��": LOCATE 17, X: PRINT "� "
 LOCATE 18, X: PRINT "� ": LOCATE 19, X: PRINT "� "
 LOCATE 20, X: PRINT "� ": LOCATE 21, X: PRINT "��"
NEXT X
DIM CMNT$(4)
FOR X = 1 TO 4
 COLOR 4, O: LOCATE X + 16, 16: LINE INPUT CMNT$(X)
NEXT X
TYPE ABTTYPE
 A AS STRING * 25
 B AS STRING * 25
 C AS STRING * 25
 D AS STRING * 25
 E AS STRING * 12
END TYPE
DIM ABT AS ABTTYPE
 ABT.A = LEFT$(CMNT$(1), 25)
 ABT.B = LEFT$(CMNT$(2), 25)
 ABT.C = LEFT$(CMNT$(3), 25)
 ABT.D = LEFT$(CMNT$(4), 25)
 ABT.E = LEFT$(ABBAT$, 12)
LET CFILE$ = LEFT$(DFILE$, 5) + ".ABT"
OPEN CFILE$ FOR RANDOM AS #2 LEN = 112
 PUT #2, P, ABT
CLOSE #2
COLOR 7, 0
FOR X = 15 TO 39
 LOCATE 16, X: PRINT " �"
 LOCATE 17, X: PRINT " �"
 LOCATE 18, X: PRINT " �"
 LOCATE 19, X: PRINT " �"
 LOCATE 20, X: PRINT " �"
 LOCATE 21, X: PRINT " �"
NEXT X
FOR X = 16 TO 20
 LOCATE X, 40: PRINT " �"
NEXT X
 LOCATE 21, 40: PRINT " �"
LOCATE 16, 3: COLOR NL(11), 0: PRINT NAME$(11)
LOCATE 16, 23: COLOR NL(12), 0: PRINT NAME$(12)
LOCATE 18, 3: COLOR NL(13), 0: PRINT NAME$(13)
LOCATE 18, 23: COLOR NL(14), 0: PRINT NAME$(14)
RETURN

CHNGBAT:
LOCATE 23, 41: COLOR 20, 3: PRINT "DOES THIS PROGRAM HAVE A DOCUMENT FILE?"
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
BATLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO BATNO
GOTO BATLOOP
BATNO:
 IF X$ = "N" OR X$ = "n" THEN
  LOCATE 23, 41: COLOR 20, 3: PRINT SPACE$(40): RETURN
 END IF
COLOR 7, 0
 LOCATE 21, 41: PRINT "��������������������������������������͹"
 LOCATE 22, 41: PRINT "��������������������������������������ͼ"
 LOCATE 22, 41: PRINT "�                                      �"
 LOCATE 23, 41: PRINT "��������������������������������������ͼ"
LOCATE 22, 43: COLOR 1, 0: PRINT "NAME OF FILE:"
LOCATE 22, 57: COLOR 4, 0: INPUT ; "", ABBAT$
 LOCATE 23, 40: COLOR 3, 3: PRINT SPACE$(41)
 LOCATE 22, 40: COLOR 0, 0: PRINT SPACE$(41)
 LOCATE 21, 41: COLOR 7, 0: PRINT "��������������������������������������ͼ"
RETURN

CEXIT:
 CHNG = 0
 DEF SEG = 0: POKE &H417, 0
 GOTO VSTART
END

CCLEAR:
FOR C = 7 TO 19
 IF C = 9 OR C = 15 THEN GOTO SKIP
 COLOR 0, 0: LOCATE C, 42: PRINT SPACE$(38)
SKIP:
NEXT C
 COLOR 3, 3: LOCATE 23, 46: PRINT SPACE$(25)
 LOCATE 5, 50: COLOR 14, 0: PRINT "SELECT FILE TO CHANGE"
 IF NAME$(P) = "" THEN LET NAME$(P) = SPACE$(16)
 PL = P + 5: PR = 3
 IF P MOD 2 = 0 THEN PL = PL - 1: PR = 23
 LOCATE PL, PR: COLOR 0, 0: PRINT SPACE$(16)
 LOCATE PL, PR: COLOR NL(P), 7: PRINT NAME$(P)
GOTO GETLOOP


GETINFO:
TYPE APPTYPE
 N AS STRING * 16
 P AS STRING * 25
 F AS STRING * 14
END TYPE
DIM APP AS APPTYPE
OPEN DFILE$ FOR RANDOM AS #1 LEN = 55
 FOR R = 1 TO 14
  GET #1, R, APP
   IF RTRIM$(APP.P) = "LIST" THEN LET NL = 5 ELSE LET NL = 2
   GN$ = APP.N: IF GN$ <> SPACE$(16) THEN LET GN$ = RTRIM$(GN$)
  NAME$(R) = GN$: PATH$(R) = APP.P: FILE$(R) = APP.F: NL(R) = NL
 NEXT R
CLOSE #1
IF CHNG = 1 THEN RETURN
GOTO START
END

SUB DOC
LET CFILE$ = LEFT$(DFILE$, 5) + ".ABT"
OPEN CFILE$ FOR RANDOM AS #2 LEN = 112
 GET #2, P, GAB
CLOSE #2
COLOR 0, 7
FILE$ = RTRIM$(GAB.E)

OPEN FILE$ FOR INPUT AS #1

WIDTH 80: COLOR 7, 0: CLS
LOCATE 1, 1: COLOR 0, 3
PRINT "                        DOCUMENT FILE ON                                        "
LOCATE 1, 42: PRINT NAME$(P)
LOCATE 23, 1: PRINT SPACE$(80)
LOCATE 23, 65: PRINT "ESC TO EXIT"

VIEW PRINT 2 TO 22
COLOR 7, 0: LOCATE 2, 1

FOR X = 1 TO 20
 IF EOF(1) = -1 THEN GOTO DLOOP
 INPUT #1, LINE$
 PRINT LINE$
NEXT X

DLOOP:
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
DO
 A$ = INKEY$
LOOP WHILE A$ = ""
 IF A$ = CHR$(0) + "P" THEN
  IF EOF(1) = -1 THEN GOTO DLOOP
  INPUT #1, LINE$
  PRINT LINE$
  GOTO DLOOP
 END IF
 IF A$ = CHR$(0) + "Q" THEN
  FOR X = 1 TO 20
   IF EOF(1) = -1 THEN GOTO DLOOP
   INPUT #1, LINE$
   PRINT LINE$
  NEXT X
  GOTO DLOOP
 END IF
 IF A$ = CHR$(27) THEN
  CLOSE #1: VIEW PRINT: CLS : EXIT SUB
 END IF
 IF A$ = CHR$(0) + "G" THEN
  SEEK #1, 1: CLS
   FOR X = 1 TO 20
    INPUT #1, LINE$: PRINT LINE$
   NEXT X
  GOTO DLOOP
 END IF
GOTO DLOOP
END

END SUB

SUB MENU
SHARED MGO
LOCATE 1, 2: COLOR 7, 4: PRINT "ABOUT"
MP = 1
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
MLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO MWHAT
GOTO MLOOP
MWHAT:
 IF X$ = CHR$(0) + "M" THEN GOTO MRIGHT
 IF X$ = CHR$(0) + "K" THEN GOTO MLEFT
 IF X$ = CHR$(13) THEN GOTO MSLCT
 IF X$ = CHR$(27) THEN GOTO MEXIT
GOTO MLOOP
MLEFT:
 W = 1
 ON MP GOTO M1, M2, M3, M4, M5
MRIGHT:
 W = 2
 ON MP GOTO M1, M2, M3, M4, M5
M1:
 LOCATE 1, 2: COLOR 0, 3: PRINT "ABOUT": LOCATE 1, 2: COLOR 4, 3: PRINT "A"
 IF W = 1 THEN LOCATE 1, 36: COLOR 7, 4: PRINT "QUIT": MP = 5
 IF W = 2 THEN LOCATE 1, 9: COLOR 7, 4: PRINT "CHANGE": MP = 2
GOTO MLOOP
M2:
 LOCATE 1, 9: COLOR 0, 3: PRINT "CHANGE": LOCATE 1, 9: COLOR 4, 3: PRINT "C"
 IF W = 1 THEN LOCATE 1, 2: COLOR 7, 4: PRINT "ABOUT": MP = 1
 IF W = 2 THEN LOCATE 1, 17: COLOR 7, 4: PRINT "DOS CMNDS": MP = 3
GOTO MLOOP
M3:
 LOCATE 1, 17: COLOR 0, 3: PRINT "DOS CMNDS": LOCATE 1, 17: COLOR 4, 3: PRINT "D"
 IF W = 1 THEN LOCATE 1, 9: COLOR 7, 4: PRINT "CHANGE": MP = 2
 IF W = 2 THEN LOCATE 1, 30: COLOR 7, 4: PRINT "HELP": MP = 4
GOTO MLOOP
M4:
 LOCATE 1, 30: COLOR 0, 3: PRINT "HELP": LOCATE 1, 30: COLOR 4, 3: PRINT "H"
 IF W = 1 THEN LOCATE 1, 17: COLOR 7, 4: PRINT "DOS CMNDS": MP = 3
 IF W = 2 THEN LOCATE 1, 36: COLOR 7, 4: PRINT "QUIT": MP = 5
GOTO MLOOP
M5:
 LOCATE 1, 36: COLOR 0, 3: PRINT "QUIT": LOCATE 1, 36: COLOR 4, 3: PRINT "Q"
 IF W = 1 THEN LOCATE 1, 30: COLOR 7, 4: PRINT "HELP": MP = 4
 IF W = 2 THEN LOCATE 1, 2: COLOR 7, 4: PRINT "ABOUT": MP = 1
GOTO MLOOP

MSLCT:
IF MP = 1 THEN GOTO ABOUT
IF MP = 2 THEN
 LET MGO = 2: EXIT SUB
END IF
IF MP = 3 THEN GOTO DOSC
IF MP = 4 THEN
 LOCATE 1, 30: COLOR 0, 3: PRINT "HELP": LOCATE 1, 30: COLOR 4, 3: PRINT "H"
 COLOR 14, 0
 LOCATE 10, 7: PRINT "��������������������������ͻ"
 LOCATE 11, 7: PRINT "�                          �"
 LOCATE 12, 7: PRINT "�                          �"
 LOCATE 13, 7: PRINT "�                          �"
 LOCATE 14, 7: PRINT "��������������������������ͼ"
 LOCATE 10, 15: COLOR 0, 6: PRINT "HELP SERVICE"
 LOCATE 12, 9: COLOR 5, 0: PRINT "HELP FILES NOT AVAILABLE!"
  WHILE INKEY$ = ""
  WEND
  FOR X = 14 TO 10 STEP -1
   COLOR 0, 0: LOCATE X, 7: PRINT SPACE$(28)
   IF X MOD 2 = 0 THEN
    IF X - 5 = P THEN RC1 = 7 ELSE RC1 = 0
     LOCATE X, 3: COLOR NL(X - 5), RC1: PRINT NAME$(X - 5)
    IF X - 4 = P THEN RC2 = 7 ELSE RC2 = 0
     LOCATE X, 23: COLOR NL(X - 4), RC2: PRINT NAME$(X - 4)
   END IF
  NEXT X
EXIT SUB
END IF
IF MP = 5 THEN
 OPEN "DO.BAT" FOR OUTPUT AS #3
  PRINT #3, "@ECHO OFF"
 CLOSE #3
 COLOR 7, 0
 END
END IF

MEXIT:
 LOCATE 1, 1: COLOR 0, 3: PRINT " ABOUT  CHANGE  DOS CMNDS    HELP  QUIT "
 COLOR 4, 3: LOCATE 1, 2: PRINT "A": LOCATE 1, 9: PRINT "C"
 LOCATE 1, 17: PRINT "D": LOCATE 1, 30: PRINT "H": LOCATE 1, 36: PRINT "Q"
EXIT SUB

ABOUT:
 LOCATE 1, 2: COLOR 0, 3: PRINT "ABOUT": LOCATE 1, 3: COLOR 4, 3: PRINT "B"
 COLOR 4, 3
 LOCATE 2, 2: PRINT "�������������Ŀ"
 LOCATE 3, 2: PRINT "�             �"
 LOCATE 4, 2: PRINT "�             �"
 LOCATE 5, 2: PRINT "�             �"
 LOCATE 6, 2: PRINT "���������������"
COLOR 7, 4: LOCATE 3, 3: PRINT "COMMENT"
COLOR 0, 3: LOCATE 5, 3: PRINT "DOCUMENT FILE"
ABP = 1

ADGET:
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
ABLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO ABWHAT
GOTO ABLOOP
ABWHAT:
 IF X$ = CHR$(0) + "P" THEN GOTO ABDOWN
 IF X$ = CHR$(0) + "H" THEN GOTO ABUP
 IF X$ = CHR$(13) THEN GOTO ABSLCT
 IF X$ = CHR$(27) THEN GOTO MEXIT
 IF X$ = CHR$(0) + "K" THEN
  LET ABE = 1: GOSUB ABEXIT: LET W = 1: GOTO M1
 END IF
 IF X$ = CHR$(0) + "M" THEN
  LET ABE = 1: GOSUB ABEXIT: LET W = 2: GOTO M1
 END IF
GOTO ABLOOP

ABSLCT:
 IF ABP = 1 THEN GOTO ABCOMNT
 IF ABP = 2 THEN GOTO ABDOC
GOTO ABLOOP

ABUP:
IF ABP = 2 THEN
 COLOR 7, 4: LOCATE 3, 3: PRINT "COMMENT"
 COLOR 0, 3: LOCATE 5, 3: PRINT "DOCUMENT FILE"
 LET ABP = 1
END IF
GOTO ABLOOP

ABDOWN:
IF ABP = 1 THEN
 COLOR 0, 3: LOCATE 3, 3: PRINT "COMMENT"
 COLOR 7, 4: LOCATE 5, 3: PRINT "DOCUMENT FILE"
 LET ABP = 2
END IF
GOTO ABLOOP

ABDOC:
 LET ABE = 1: GOSUB ABEXIT
 LET MGO = 5
EXIT SUB

ABCOMNT:
LET ABE = 1: GOSUB ABEXIT
COLOR 14, 0
LOCATE 7, 6: PRINT "�������������������������ͻ"
LOCATE 8, 6: PRINT "�                         �"
LOCATE 9, 6: PRINT "�                         �"
LOCATE 10, 6: PRINT "�                         �"
LOCATE 11, 6: PRINT "�                         �"
LOCATE 12, 6: PRINT "�������������������������ͼ"
LOCATE 7, 8: COLOR 0, 14: PRINT "ABOUT: "; SPACE$(16)
LOCATE 7, 15: PRINT NAME$(P)
LET CFILE$ = LEFT$(DFILE$, 5) + ".ABT"
OPEN CFILE$ FOR RANDOM AS #2 LEN = 112
 GET #2, P, GAB
CLOSE #2
COLOR 4, 0
LOCATE 8, 7: PRINT GAB.A
LOCATE 9, 7: PRINT GAB.B
LOCATE 10, 7: PRINT GAB.C
LOCATE 11, 7: PRINT GAB.D
WHILE INKEY$ = ""
WEND
FOR X = 12 TO 7 STEP -1
 COLOR 0, 0: LOCATE X, 6: PRINT SPACE$(27)
 IF X MOD 2 = 0 THEN
  IF X - 5 = P THEN RC1 = 7 ELSE RC1 = 0
   LOCATE X, 3: COLOR NL(X - 5), RC1: PRINT NAME$(X - 5)
  IF X - 4 = P THEN RC2 = 7 ELSE RC2 = 0
   LOCATE X, 23: COLOR NL(X - 4), RC2: PRINT NAME$(X - 4)
 END IF
NEXT X
EXIT SUB

ABEXIT:
COLOR 7, 0
FOR X = 6 TO 2 STEP -1
 LOCATE X, 2: PRINT SPACE$(15)
NEXT X
 IF P = 1 THEN LET ABCOL = 7 ELSE LET ABCOL = 0
 COLOR 2, ABCOL: LOCATE 6, 3: PRINT NAME$(1)
IF ABE = 1 THEN RETURN
EXIT SUB

DOSC:
 LOCATE 1, 17: COLOR 0, 3: PRINT "DOS CMNDS": LOCATE 1, 17: COLOR 4, 3: PRINT "D"
DIM DOS$(9)
DOS$(1) = "EDITOR": DOS$(2) = "BACKUP": DOS$(3) = "DELETE"
DOS$(4) = "DIR": DOS$(5) = "TREE": DOS$(6) = "SET"
DOS$(7) = "MEM": DOS$(8) = "DATE+TIME": DOS$(9) = "OTHER"
 COLOR 4, 3
 LOCATE 2, 19: PRINT "���������Ŀ"
 LOCATE 3, 19: PRINT "�         �"
 LOCATE 4, 19: PRINT "���������Ĵ"
 LOCATE 5, 19: PRINT "�         �"
 LOCATE 6, 19: PRINT "�         �"
 LOCATE 7, 19: PRINT "�         �"
 LOCATE 8, 19: PRINT "�         �"
 LOCATE 9, 19: PRINT "�         �"
 LOCATE 10, 19: PRINT "�         �"
 LOCATE 11, 19: PRINT "�         �"
 LOCATE 12, 19: PRINT "�         �"
 LOCATE 13, 19: PRINT "�����������"
COLOR 7, 4
FOR X = 1 TO 9
 LOCATE X + 3, 20
 IF X = 1 THEN LOCATE 3, 20
 PRINT DOS$(X): COLOR 0, 3
NEXT X
DCP = 1: ABDC = 2

DCGET:
DO
 A$ = INKEY$
LOOP UNTIL A$ = ""
DCLOOP:
 X$ = INKEY$
 IF X$ <> "" THEN GOTO DCWHAT
GOTO DCLOOP
DCWHAT:
 IF X$ = CHR$(0) + "P" THEN GOTO DCDOWN
 IF X$ = CHR$(0) + "H" THEN GOTO DCUP
 IF X$ = CHR$(13) THEN GOTO DCSLCT
 IF X$ = CHR$(27) THEN GOTO DCEXIT
 IF X$ = CHR$(0) + "K" THEN
  LET DCE = 1: GOSUB DCEXIT: LET W = 1: GOTO M3
 END IF
 IF X$ = CHR$(0) + "M" THEN
  LET DCE = 1: GOSUB DCEXIT: LET W = 2: GOTO M3
 END IF
GOTO DCLOOP

DCSLCT:
IF DCP = 1 THEN
 LET DCE = 1: GOSUB DCEXIT
 COLOR 14, 0
 LOCATE 10, 7: PRINT "������������������������ͻ"
 LOCATE 11, 7: PRINT "�                        �"
 LOCATE 12, 7: PRINT "������������������������Ķ"
 LOCATE 13, 7: PRINT "�                        �"
 LOCATE 14, 7: PRINT "�                        �"
 LOCATE 15, 7: PRINT "������������������������ͼ"
 LOCATE 11, 14: COLOR 5, 0: PRINT "FILE TO EDIT"
 LOCATE 13, 9: COLOR 1, 0: PRINT "PATH:"
 LOCATE 14, 9: COLOR 1, 0: PRINT "FILE:"
 LOCATE 13, 15: COLOR 4, 0: INPUT ; "", EPATH$
 LOCATE 14, 15: COLOR 4, 0: INPUT ; "", EFILE$
 SHEDIT$ = "EDITOR " + EFILE$
 CHDIR EPATH$: WIDTH 80: SHELL SHEDIT$: MGO = 3: EXIT SUB
END IF
IF DCP = 2 THEN
 LET DCE = 1: GOSUB DCEXIT
 COLOR 14, 0
 LOCATE 10, 7: PRINT "��������������������������ͻ"
 LOCATE 11, 7: PRINT "�                          �"
 LOCATE 12, 7: PRINT "�                          �"
 LOCATE 13, 7: PRINT "�                          �"
 LOCATE 14, 7: PRINT "��������������������������ͼ"
 LOCATE 10, 14: COLOR 0, 6: PRINT "BACKUP SERVICE"
 LOCATE 12, 9: COLOR 5, 0: PRINT "BACKUP "; NAME$(P); "?"
YNGET:
 DO
  A$ = INKEY$
 LOOP UNTIL A$ = ""
 DO
  X$ = INKEY$
 LOOP WHILE X$ = ""
IF YN = 1 THEN RETURN
 IF X$ = "Y" OR X$ = "y" THEN LET SPATH$ = PATH$(P): SKP = 1
 IF SKP <> 1 THEN
  LOCATE 12, 8: PRINT SPACE$(25)
  LOCATE 12, 9: COLOR 1, 0: PRINT "ENTER SOURCE PATH:"
  LOCATE 13, 9: COLOR 4, 0: INPUT ; "", SPATH$
 END IF
  LOCATE 13, 9: PRINT SPACE$(25)
  LOCATE 12, 9: COLOR 1, 0: PRINT "ENTER DESTINATION DRIVE:"
  LOCATE 13, 9: COLOR 4, 0: INPUT ; "", DRIVE$
 IF SKP <> 1 THEN
  LOCATE 13, 9: PRINT SPACE$(25)
  LOCATE 12, 9: COLOR 1, 0: PRINT "BACKUP SUBDIRECTORIES ? "
  LET YN = 1: GOSUB YNGET: IF X$ = "y" THEN LET S$ = "/S" ELSE LET S$ = ""
 END IF
  LOCATE 13, 9: PRINT SPACE$(25)
  LOCATE 12, 9: COLOR 1, 0: PRINT "FORMAT BEFORE BACKUP ?  "
  LET YN = 1: GOSUB YNGET: IF X$ = "y" THEN LET F$ = "/F" ELSE LET F$ = ""
 LET BAKSHL$ = "BACKUP " + SPATH$ + " " + DRIVE$ + " " + S$ + " " + F$
 WIDTH 80: CLS : SHELL BAKSHL$: LET MGO = 3: EXIT SUB
END IF
IF DCP = 3 THEN
 LET DCE = 1: GOSUB DCEXIT
 COLOR 14, 0
 LOCATE 10, 7: PRINT "��������������������������ͻ"
 LOCATE 11, 7: PRINT "�                          �"
 LOCATE 12, 7: PRINT "�                          �"
 LOCATE 13, 7: PRINT "�                          �"
 LOCATE 14, 7: PRINT "��������������������������ͼ"
 LOCATE 10, 14: COLOR 0, 6: PRINT "DELETE SERVICE"
 LOCATE 12, 9: COLOR 5, 0: PRINT "DELETE "; NAME$(P); "?"
 DO
  A$ = INKEY$
 LOOP UNTIL A$ = ""
 DO
  X$ = INKEY$
 LOOP WHILE X$ = ""
 IF X$ = "Y" OR X$ = "y" THEN LET DPATH$ = RTRIM$(PATH$(P)) + "\*.*": SKP = 1
 IF SKP <> 1 THEN
  LOCATE 12, 8: PRINT SPACE$(25)
  LOCATE 12, 9: COLOR 1, 0: PRINT "ENTER FILE & PATH:"
  LOCATE 13, 9: COLOR 4, 0: INPUT ; "", DPATH$
 END IF
 SHELL "DEL " + DPATH$
 END
 LET MGO = 3: EXIT SUB
END IF

DCUP:
DED = 2
 IF DCP = 1 THEN GOTO DCGET
 IF DCP = 2 THEN DED = 1
 COLOR 0, 3: LOCATE DCP + 3, 20: PRINT DOS$(DCP)
 COLOR 7, 4: LOCATE DCP + DED, 20: PRINT DOS$(DCP - 1)
DCP = DCP - 1
GOTO DCGET

DCDOWN:
DED = 3
 IF DCP = 9 THEN GOTO DCGET
 IF DCP = 1 THEN DED = 2
 COLOR 0, 3: LOCATE DCP + DED, 20: PRINT DOS$(DCP)
 COLOR 7, 4: LOCATE DCP + 4, 20: PRINT DOS$(DCP + 1)
DCP = DCP + 1
GOTO DCGET

DCEXIT:
COLOR 7, 0
FOR X = 13 TO 2 STEP -1
 LOCATE X, 19: PRINT SPACE$(11)
 IF X MOD 2 = 0 AND X > 5 THEN
  LOCATE X, 23: COLOR NL(X - 4), 0: PRINT NAME$(X - 4)
 END IF
NEXT X
 PL = P: PR = 3
 IF P MOD 2 = 0 THEN LET PL = PL - 1: PR = 23
 LOCATE PL + 5, PR: COLOR NL(P), 7: PRINT NAME$(P)
IF DCE = 1 THEN RETURN
EXIT SUB

END SUB
