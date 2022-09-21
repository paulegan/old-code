'           *******   Written by Paul Egan 21-12-1993  ********
'
'                      This program was written for
'                   PcPlus, Wilf's Programmers' Workshop,
'                           Program Challenge.

SCREEN 2
LET Z% = 0: S = 10
 DO
 
  DO
   A$ = INKEY$
  LOOP WHILE A$ = ""

  IF LEN(A$) > 1 THEN
   LET B% = ASC(RIGHT$(A$, 1)): Z% = B% MOD 2
   PRESET STEP((B% - 76) * S * 2 * Z%, ((B% / 4) - 19) * (1 - Z%) * S), 1
  END IF

 LOOP UNTIL ASC(A$) = 27
END

