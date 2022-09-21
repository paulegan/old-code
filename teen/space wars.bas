
'               *****            SPACE WARS            *****
'                              Lumpy  3-10-95

 DECLARE SUB dealWith (a$)
 DECLARE SUB moveShipOne ()
 DECLARE SUB moveShipTwo ()
 DECLARE SUB moveBullets ()
 DECLARE SUB blowUp (ship)
 DECLARE SUB checkPower (dead)

 CONST shipOne$ = "c2u1c1f6l12e6", shipTwo$ = "c1u1c2f6l12e6"
 CONST blast$ = "bd6l2r4g1l2"
 DIM SHARED s1x, s1y, s1Dir, s1xInc, s1yInc, s1Power AS INTEGER
 DIM SHARED b1x, b1y, b1xInc, b1yInc AS INTEGER
 DIM SHARED s2x, s2y, s2Dir, s2xInc, s2yInc, s2Power AS INTEGER
 DIM SHARED b2x, b2y, b2xInc, b2yInc AS INTEGER

 s1x = 100: s1y = 100: s1Power = 20: s2x = 150: s2y = 150: s2Power = 20
 b1x = 340: b1y = 210: b2x = 340: b2y = 210

 SCREEN 1
 COLOR 0, 1
 LINE (20, 2)-STEP(20, 4), 1, BF
 LINE (280, 2)-STEP(20, 4), 2, BF

 DO
  a$ = RIGHT$(INKEY$, 1)
  IF LEN(a$) THEN dealWith (a$)

  CALL moveBullets
  CALL moveShipOne
  CALL moveShipTwo
  CALL checkPower(dead)
 LOOP UNTIL a$ = CHR$(27) OR dead

 blowUp (dead)
END

SUB blowUp (ship)

 IF ship = 2 OR ship = 0 THEN
  LINE (s2x - 8, s2y - 7)-(s2x + 8, s2y + 7), 0, BF
  FOR x = 3 TO 11 STEP 4
    s2x = s2x + s2xInc: s2y = s2y + s2yInc
   FOR c = 2 TO 0 STEP -2
    PSET (s2x, s2y), 0
    DRAW "c=" + VARPTR$(c) + "be=" + VARPTR$(x) + "f6bg=" + VARPTR$(x)
    DRAW "l12bh=" + VARPTR$(x) + "e6"
   NEXT c
  NEXT x
 END IF

 IF ship = 1 OR ship = 0 THEN
  LINE (s1x - 8, s1y - 7)-(s1x + 8, s1y + 7), 0, BF
  FOR x = 3 TO 11 STEP 4
    s1x = s1x + s1xInc: s1y = s1y + s1yInc
   FOR c = 1 TO 0 STEP -1
    PSET (s1x, s1y), 0
    DRAW "c=" + VARPTR$(c) + "be=" + VARPTR$(x) + "f6bg=" + VARPTR$(x)
    DRAW "l12bh=" + VARPTR$(x) + "e6"
   NEXT c
  NEXT x
 END IF

END SUB

SUB checkPower (dead)

 IF ABS(b2x - s1x) + ABS(b2y - s1y) < 10 THEN
  PSET (s1x, s1y), 0
  DRAW "ta=" + VARPTR$(s1Dir) + "c3u1f6l12e6"
  s1Power = s1Power - 2
 END IF
 IF ABS(b1x - s2x) + ABS(b1y - s2y) < 10 THEN
  PSET (s2x, s2y), 0
  DRAW "ta=" + VARPTR$(s2Dir) + "c3u1f6l12e6"
  s2Power = s2Power - 2
 END IF

 IF s1Power < 0 THEN dead = 1
 IF s2Power < 0 THEN dead = 2

 LINE (40, 2)-STEP(s1Power - 20, 4), 0, BF
 LINE (300, 2)-STEP(s2Power - 20, 4), 0, BF

END SUB

SUB dealWith (a$)

 IF a$ = "K" THEN s1Dir = (s1Dir + 30) MOD 360
 IF a$ = "M" THEN s1Dir = (s1Dir + 330) MOD 360
 IF a$ = "H" THEN
  PSET (s1x, s1y), 0: DRAW "c2ta=" + VARPTR$(s1Dir) + blast$
  s1xInc = s1xInc - 2 * SIN(.01745 * s1Dir)
  s1yInc = s1yInc - 2 * COS(.01745 * s1Dir)
 END IF
 IF a$ = CHR$(13) THEN
  CIRCLE (b1x, b1y), 1, 0
  b1x = s1x: b1y = s1y
  b1xInc = -15 * SIN(.01745 * s1Dir)
  b1yInc = -15 * COS(.01745 * s1Dir)
 END IF

 IF a$ = "a" THEN s2Dir = (s2Dir + 30) MOD 360
 IF a$ = "d" THEN s2Dir = (s2Dir + 330) MOD 360
 IF a$ = "w" THEN
  PSET (s2x, s2y), 0: DRAW "c1ta=" + VARPTR$(s2Dir) + blast$
  s2xInc = s2xInc - 2 * SIN(.01745 * s2Dir)
  s2yInc = s2yInc - 2 * COS(.01745 * s2Dir)
 END IF
 IF a$ = " " THEN
  CIRCLE (b2x, b2y), 1, 0
  b2x = s2x: b2y = s2y
  b2xInc = -15 * SIN(.01745 * s2Dir)
  b2yInc = -15 * COS(.01745 * s2Dir)
 END IF

END SUB

SUB moveBullets

 CIRCLE (b1x, b1y), 1, 0
 b1x = b1x + b1xInc: b1y = b1y + b1yInc
 CIRCLE (b1x, b1y), 1, 3

 CIRCLE (b2x, b2y), 1, 0
 b2x = b2x + b2xInc: b2y = b2y + b2yInc
 CIRCLE (b2x, b2y), 1, 3

END SUB

SUB moveShipOne

 LINE (s1x - 8, s1y - 7)-(s1x + 8, s1y + 7), 0, BF
 s1x = s1x + s1xInc: s1y = s1y + s1yInc
 IF s1x < 10 OR s1x > 310 THEN s1xInc = -s1xInc
 IF s1y < 20 OR s1y > 190 THEN s1yInc = -s1yInc
 PSET (s1x, s1y), 0
 DRAW "ta=" + VARPTR$(s1Dir) + shipOne$

END SUB

SUB moveShipTwo

 LINE (s2x - 8, s2y - 7)-(s2x + 8, s2y + 7), 0, BF
 s2x = s2x + s2xInc: s2y = s2y + s2yInc
 IF s2x < 10 OR s2x > 310 THEN s2xInc = -s2xInc
 IF s2y < 20 OR s2y > 190 THEN s2yInc = -s2yInc
 PSET (s2x, s2y), 0
 DRAW "ta=" + VARPTR$(s2Dir) + shipTwo$

END SUB

