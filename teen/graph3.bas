REM grapher
SCREEN 1
Y = 90
'LINE (10, 10)-(10, 170), 2
'LINE (350, Y)-(10, Y), 2
PSET (10, 170)
COLOR 0, 2

GOTO SKIP

FOR X = 0 TO 3.14 * 3 STEP .07
 PSET (10 + X * (38 - X), Y + 80), 1
 LINE STEP(0, 0)-(1 + X * (40 - X), Y - SIN((X ^ X / 5)) * 70), 3
NEXT X

SKIP:

PSET (10, 90)
FOR X = 3.14 TO 7.28 * 4 STEP .08
 LINE STEP(0, 0)-(1 + X * 20, Y - SIN((X)) * 30), 1
 PSET (10 + X * 20, Y + 80), 1
NEXT X

END

