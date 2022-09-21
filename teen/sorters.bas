REM sort tester
 DECLARE SUB selectionSort (array())
 DECLARE SUB bubbleSort (array())
 CONST numOfTimes = 3, arraySize = 6
 DIM numbers(arraySize), times(numOfTimes)
 RANDOMIZE TIMER

CLS

FOR counter = 1 TO numOfTimes
 FOR x = 1 TO arraySize
  numbers(x) = INT(101 * RND)
  PRINT numbers(x);
 NEXT x
 PRINT

 time(counter) = TIMER
  CALL selectionSort(numbers())
 time(counter) = TIMER - time(counter)

 FOR x = 1 TO arraySize
  PRINT numbers(x);
 NEXT x
 PRINT
 PRINT time(counter)
NEXT counter

FOR x = 1 TO numOfTimes
 totalTime = totalTime + time(x)
NEXT x
PRINT "the average time for an array of size";
PRINT arraySize; "was:"; totalTime / numOfTimes
END

SUB bubbleSort (array())

 DO
  swappedANumber = 0
  FOR n = 0 TO UBOUND(array) - 1
    IF array(n) < array(n + 1) THEN
      SWAP array(n), array(n + 1)
      swappedANumber = 1
    END IF
  NEXT n
 LOOP WHILE swappedANumber

END SUB

SUB selectionSort (array())
 size = UBOUND(array)

 FOR x = 1 TO size - 1
  FOR y = x + 1 TO size
   IF array(x) < array(y) THEN SWAP array(x), array(y)
  NEXT y
 NEXT x

END SUB

