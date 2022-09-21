DEFINT A-Z
DIM PRIME(330) AS INTEGER
CLS
COLOR 2, 0: PRINT "THE FIRST 329 PRIME NUMBERS ARE:"
COLOR 3, 0: PRINT 2
'GOTO AllFactors

P = 0: N = 3: PRIME(0) = 2: PRIME(1) = 3: PRIME(2) = 5: PRIME(3) = 7:
T = TIMER
 DO
  F = P \ (3 + P / 17)'   <-- BE CAREFUL OF THIS!!
   DO UNTIL (N MOD PRIME(F)) = 0 OR F = 0
    F = F - 1
   LOOP
  IF F < 1 THEN
   P = P + 1: PRIME(P) = N
   LOCATE (P MOD 22) + 2, (P \ 22) * 5 + 1: PRINT N
  END IF
  N = N + 2
 LOOP UNTIL P = 329
COLOR 7, 0: LOCATE 1, 55: PRINT "TIME TAKEN:"; INT(TIMER - T); "secs"
END


AllFactors:
P = 0: N = 3
T = TIMER
 DO
  F = INT(N ^ .5) + 1
   DO
    F = F - 1
   LOOP UNTIL (N MOD F) = 0 OR F = 0
  IF F < 2 THEN
   P = P + 1
   LOCATE (P MOD 22) + 2, (P \ 22) * 5 + 1: PRINT N
  END IF
  N = N + 2
 LOOP UNTIL P = 329
COLOR 7, 0: LOCATE 1, 55: PRINT "TIME TAKEN:"; INT(TIMER - T); "secs"
END

