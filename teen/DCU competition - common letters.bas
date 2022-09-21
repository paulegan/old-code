'                              Common Letters

'                       A solution to problem 3 of the
'               2nd All Ireland Schools Programming Competition

'                         by Paul Egan   4-2-1995

  DEFINT A-Z

  INPUT "Enter two words : ", theWords$
  theMiddle = INSTR(theWords$, " ")
  theEnd = LEN(theWords$)

  FOR x = 1 TO theMiddle               'Get the common letters:
   FOR y = theMiddle TO theEnd
    IF MID$(theWords$, x, 1) = MID$(theWords$, y, 1) THEN
     commonLetters$ = commonLetters$ + MID$(theWords$, x, 1)
    END IF
   NEXT y
  NEXT x

  FOR x = 1 TO theEnd               'Print each letter:
   theLetter$ = MID$(theWords$, x, 1)
   IF INSTR(commonLetters$, theLetter$) THEN theLetter$ = UCASE$(theLetter$)
   PRINT theLetter$;
  NEXT x

END


