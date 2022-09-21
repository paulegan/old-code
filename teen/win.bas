REM Windows Shell Loader
 DIM Shel AS STRING * 20
 IF COMMAND$ = "" THEN LET Shel = "PROGMAN.EXE" ELSE LET Shel = COMMAND$

 OPEN "SYSTEM.INI" FOR BINARY AS #2
  FOR X = 1 TO 20
   LET L$ = MID$(Shel, X, 1)
   PUT #2, 25 + X, L$
  NEXT X
 CLOSE #2
 CHAIN "WINDOWS.COM"
END

