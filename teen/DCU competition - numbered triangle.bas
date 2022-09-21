'                              Number Triangle

'                       A solution to problem 5 of the
'               2nd All Ireland Schools Programming Competition

'                         by Paul Egan   5-2-1995


 DECLARE SUB getSolutions (pointer%, theSum%)
 DEFINT A-Z

 TYPE tree
  item AS INTEGER
  left AS INTEGER
  right AS INTEGER
 END TYPE

 CLS
 INPUT "Enter a number triangle : ", numOfRows
  numOfSolutions = 2 ^ (numOfRows - 1)
  DIM SHARED solutions(numOfSolutions), elementNum
  DIM SHARED triangle((numOfRows / 2) * (numOfRows + 1)) AS tree

 FOR r = 1 TO numOfRows                 'Put the triangle info into a tree:
  INPUT "", row$
   FOR e = 1 TO r
    x = INSTR(row$, " ") - 1: IF x < 1 THEN x = LEN(row$)
    elementNum = r / 2 * (r - 1) + e
     triangle(elementNum).item = VAL(LEFT$(row$, x))
     triangle(elementNum).left = elementNum + r
     triangle(elementNum).right = elementNum + r + 1
    row$ = MID$(row$, x + 2)
   NEXT e
 NEXT r
 
  CALL getSolutions(1, theSum)          '<-- recusive procedure!

 FOR x = 1 TO numOfSolutions            'Get the largest solution:
  IF theSolution < solutions(x) THEN theSolution = solutions(x)
 NEXT x

 PRINT : PRINT "The solution is"; STR$(theSolution); "."

END

SUB getSolutions (pointer, theSum)
 STATIC s

 IF triangle(pointer).left > elementNum THEN
  s = s + 1
  solutions(s) = theSum + triangle(pointer).item
  EXIT SUB
 END IF

 CALL getSolutions(triangle(pointer).left, theSum + triangle(pointer).item)
 CALL getSolutions(triangle(pointer).right, theSum + triangle(pointer).item)

END SUB

