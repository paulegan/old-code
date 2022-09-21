SCREEN 1

LOCATE 5, 4: PRINT "    3x"
LOCATE 6, 4: PRINT "x2 - 5x + 4"
LINE (24, 39)-STEP(85, 0), 3

S = 10: T = 10
ZERO = 120

LINE (ZERO, 1)-(ZERO, 200), 2
LINE (ZERO - 100, 100)-(ZERO + 100, 100), 2
FOR X = -10 TO 10
 LINE (ZERO + X * S, 98)-(ZERO + X * S, 102), 2
 LINE (ZERO - 3, 100 + X * T)-(ZERO + 3, 100 + X * T), 2
NEXT X

PSET (ZERO, 100), 3
LINE (ZERO + 1 * S, 10)-(ZERO + 1 * S, 170), 3
LINE (ZERO + 4 * S, 10)-(ZERO + 4 * S, 170), 3

FOR X = 4.2 TO 15 STEP .2
 PSET (ZERO + X * S, 100 - ((3 * X) / (X * X - (5 * X) + 4)) * T), 1
NEXT

FOR X = 1.1 TO 3.8 STEP .1
 PSET (ZERO + X * S, 100 - ((3 * X) / (X * X - (5 * X) + 4)) * T), 1
NEXT

FOR X = -10 TO .9 STEP .1
 PSET (ZERO + X * S, 100 - ((3 * X) / (X * X - (5 * X) + 4)) * T), 1
NEXT

END

