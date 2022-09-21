'                                Word Chain

'                       A solution to problem 4 of the
'               2nd All Ireland Schools Programming Competition

'                         by Paul Egan   5-2-1995

 DEFINT A-Z

 CLS
 INPUT "Enter the list of words : ", numOfWords
 INPUT "", theWords$

 DIM word$(numOfWords)                          'separate the words:
 FOR x = 0 TO numOfWords - 2
  word$(x) = LEFT$(theWords$, INSTR(theWords$, " ") - 1)
  theWords$ = MID$(theWords$, INSTR(theWords$, " ") + 1)
 NEXT x
  word$(numOfWords - 1) = theWords$

 DO                                             'find a word chain:
  fisrtWord = firstWord + 1
  wordChain$ = word$(firstWord): numInChain = 1: n = 0
  DO
   n = n + 1: w = n MOD numOfWords
    IF INSTR(wordChain$, word$(w)) = 0 THEN
     IF LEFT$(word$(w), 1) = RIGHT$(wordChain$, 1) THEN
      IF numInChain = 3 THEN
       IF RIGHT$(word$(w), 1) = LEFT$(wordChain$, 1) THEN
        wordChain$ = wordChain$ + "-" + word$(w)
        numInChain = numInChain + 1
       END IF
      ELSEIF numInChain <> 3 THEN
        wordChain$ = wordChain$ + "-" + word$(w)
        numInChain = numInChain + 1
      END IF
     END IF
    END IF
  LOOP UNTIL numInChain = 4 OR n = 4 * numOfWords
 LOOP UNTIL firstWord = numOfWords - 1 OR numInChain = 4

 PRINT : PRINT "A four word chain is : "; wordChain$

END


