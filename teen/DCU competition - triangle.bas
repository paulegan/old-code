'                            Is this a Triangle

'                       A solution to problem 2 of the
'               2nd All Ireland Schools Programming Competition

'                         by Paul Egan   4-2-1995

 DECLARE SUB bubbleSort (number%())
 DIM side(2) AS INTEGER

 INPUT "Enter three sides : ", side(0), side(1), side(2)    '<-- Requires a
                                                            ' comma between
  CALL bubbleSort(side())                                   ' the numbers!
 
  IF (side(1) + side(2)) <= side(0) THEN
   PRINT "impossible"
   ELSEIF side(0) = side(2) THEN PRINT "equilateral"
   ELSEIF side(0) = side(1) OR side(1) = side(2) THEN PRINT "isosceles"
   ELSE
    PRINT "possible"
  END IF

END

DEFINT A-Z
SUB bubbleSort (number%())

 DO
  swappedANumber = 0
  FOR n = 0 TO UBOUND(number%) - 1
    IF number(n) < number(n + 1) THEN
      SWAP number(n), number(n + 1)
      swappedANumber = 1
    END IF
  NEXT n
 LOOP WHILE swappedANumber

END SUB

