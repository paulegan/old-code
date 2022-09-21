DECLARE SUB GETFILES (SEARCH$, NUMFOUND%, TAG$())
DIM TAG$(100)

DO
 NUMFOUND% = 0
 CLS

 'INPUT "Enter File Spec:  ", SEARCH$
 INPUT "Enter directory to search:  ", DIRNAME$
 IF DIRNAME$ <> "" THEN CHDIR DIRNAME$
 SEARCH$ = "*.*"

   CALL GETFILES(SEARCH$, NUMFOUND%, TAG$())

 PRINT "The"; NUMFOUND%; "files found were: "
  FOR I = 0 TO 1 + NUMFOUND%
    IF NUMFOUND% > 8 THEN
     LOCATE (I MOD (NUMFOUND% / 5)) + 4, (I \ (NUMFOUND% / 5)) * 14 + 4
    ELSE
     LOCATE I + 4, 4
    END IF
    PRINT LEFT$(TAG$(I), INSTR(TAG$(I), CHR$(0)))
  NEXT

 DO
  A$ = INKEY$
 LOOP WHILE A$ = ""

LOOP UNTIL A$ = CHR$(27)
END

SUB GETFILES (SEARCH$, NUMFOUND%, TAG$()) STATIC

       DEFINT A-Z
       DIR.RETURN.CODE% = 0
       '$DYNAMIC    
          DIM FILES$(2)
       '$STATIC                     
       COUNT = &HFF00 OR 0
       I = INT(VARPTR(FILES$(0)))
      
CALL DIR(SEARCH$, I, COUNT, DIR.RETURN.CODE)
      
       IF COUNT < 1 THEN PRINT "No Files Found!": ERASE FILES$: EXIT SUB
      
       REDIM FILES$(COUNT + 1)
       FOR I = 0 TO COUNT + 1
           FILES$(I) = SPACE$(22)
       NEXT

       COUNT = 0
       I = INT(VARPTR(FILES$(0)))

CALL DIR(SEARCH$, I, COUNT, DIR.RETURN.CODE%)

       FOR J = 0 TO COUNT + 1
         TAG$(J) = FILES$(J)
       NEXT
       NUMFOUND% = COUNT

       ERASE FILES$

END SUB

