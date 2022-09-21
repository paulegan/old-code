REM Roman Numeral to Decimal Converter
REM Written by Paul Egan XXIV-II-MCMXCIV
REM For D.C.U. Irish Schools' Computer Programming Competition
CLS

INPUT " Enter roman numeral:", A$
 IF A$ = "QUIT" THEN END
 DIM NUM(LEN(A$) + 1) AS INTEGER

FOR X = 1 TO LEN(A$) + 1
 IF MID$(A$, X, 1) = "M" THEN NUM(X) = 1000
 IF MID$(A$, X, 1) = "D" THEN NUM(X) = 500
 IF MID$(A$, X, 1) = "C" THEN NUM(X) = 100
 IF MID$(A$, X, 1) = "L" THEN NUM(X) = 50
 IF MID$(A$, X, 1) = "X" THEN NUM(X) = 10
 IF MID$(A$, X, 1) = "V" THEN NUM(X) = 5
 IF MID$(A$, X, 1) = "I" THEN NUM(X) = 1
NEXT X

FOR X = 0 TO LEN(A$)
 IF NUM(X) < NUM(X + 1) THEN DEC% = DEC% - NUM(X) ELSE DEC% = DEC% + NUM(X)
NEXT X

PRINT A$; " ="; DEC%

END

