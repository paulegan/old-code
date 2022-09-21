'                                Factorials

'                       A solution to problem 1 of the
'               2nd All Ireland Schools Programming Competition

'                         by Paul Egan   3-2-1995

 DEFINT A-Z

 INPUT " Enter a Number : ", number
 
  numFactorial = 1
  FOR counter = 1 TO number
   numFactorial = numFactorial * counter
  NEXT

 PRINT STR$(number) + "! ="; numFactorial

END

