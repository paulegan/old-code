
'         ∞∞∞±±±≤≤≤€€€ INCLUDE FILE FOR LUMPY2 LIBRARY €€€≤≤≤±±±∞∞∞

TYPE RegType
     ax    AS INTEGER
     bx    AS INTEGER
     cx    AS INTEGER
     dx    AS INTEGER
     bp    AS INTEGER
     si    AS INTEGER
     di    AS INTEGER
     flags AS INTEGER
END TYPE

TYPE RegTypeX
     ax    AS INTEGER
     bx    AS INTEGER
     cx    AS INTEGER
     dx    AS INTEGER
     bp    AS INTEGER
     si    AS INTEGER
     di    AS INTEGER
     flags AS INTEGER
     ds    AS INTEGER
     es    AS INTEGER
END TYPE

TYPE MouseEvent
	X AS INTEGER
	Y AS INTEGER
	buttons AS INTEGER
END TYPE

TYPE BitMap                 ' Bitmap structure type
	screen1 AS INTEGER
	screen2 AS INTEGER
	screen3 AS INTEGER
	screen4 AS INTEGER
	screen5 AS INTEGER
	screen6 AS INTEGER
	screen7 AS INTEGER
	screen8 AS INTEGER      ' 16 integers (each 16 bits) to represent
	screen9 AS INTEGER      ' the screen mask bitmap. (I suggest setting
	screen10 AS INTEGER     ' all bits to 1 (-1 or NOT 0 in Basic's
	screen11 AS INTEGER     ' method of storing integers) unless you
	screen12 AS INTEGER     ' want a solid over-writing type pointer.)
	screen13 AS INTEGER
	screen14 AS INTEGER
	screen15 AS INTEGER
	screen16 AS INTEGER
	ptr1 AS INTEGER
	ptr2 AS INTEGER
	ptr3 AS INTEGER
	ptr4 AS INTEGER
	ptr5 AS INTEGER
	ptr6 AS INTEGER
	ptr7 AS INTEGER
	ptr8 AS INTEGER         ' 16 integers (each 16 bits) to represent
	ptr9 AS INTEGER         ' the cursor mask bitmap. (This is the
	ptr10 AS INTEGER        ' actual bitmap which determines the shape
	ptr11 AS INTEGER        ' of the mouse pointer.)
	ptr12 AS INTEGER
	ptr13 AS INTEGER
	ptr14 AS INTEGER
	ptr15 AS INTEGER
	ptr16 AS INTEGER
END TYPE



DEFINT A-Z

DECLARE SUB INTERRUPT (intnum AS INTEGER, inreg AS RegType, outreg AS RegType)
DECLARE SUB INTERRUPTX (intnum AS INTEGER, inreg AS RegTypeX, outreg AS RegTypeX)
DECLARE SUB ABSOLUTE (address AS INTEGER)
DECLARE SUB INT86OLD (intnum AS INTEGER, inarray() AS INTEGER, outarray() AS INTEGER)
DECLARE SUB INT86XOLD (intnum AS INTEGER, inarray() AS INTEGER, outarray() AS INTEGER)

DECLARE FUNCTION MouseInit (mode AS INTEGER)
DECLARE SUB MouseSetPos (X AS INTEGER, Y AS INTEGER)
DECLARE SUB MouseHide ()
DECLARE SUB MouseShow ()
DECLARE FUNCTION MouseMove (event AS MouseEvent)
DECLARE FUNCTION MouseButton (event AS MouseEvent, debounce AS INTEGER)
DECLARE FUNCTION MouseNewEvent (event AS MouseEvent)
DECLARE SUB MouseGetInfo (event AS MouseEvent)
DECLARE SUB MouseTextPtr (scl AS INTEGER, sch AS STRING, cl AS INTEGER, ch AS STRING)
DECLARE SUB MouseGraphicsPtr (hotx AS INTEGER, hoty AS INTEGER, map AS BitMap)

DECLARE FUNCTION GetBFont% ()
DECLARE SUB BFont (BYVAL FontNr%)
DECLARE SUB G1Banner (St$, X%, Y%, Xmul%, Ymul%)
DECLARE SUB G1Box (BYVAL X1%, BYVAL Y1%, BYVAL X2%, BYVAL Y2%, BYVAL Fill%)
DECLARE SUB G1Cls ()
DECLARE SUB G1Color (BYVAL Foregnd%, BYVAL Backgnd%)
DECLARE SUB G1GetColor (Foregnd%, Backgnd%)
DECLARE SUB G1Ellipse (CenterX%, CenterY%, XRadius%, YRadius%)
DECLARE SUB G1Line (BYVAL X1%, BYVAL Y1%, BYVAL X2%, BYVAL Y2%)
DECLARE SUB G1PaletteB (BYVAL PaletteNr%)
DECLARE SUB G1Plot (BYVAL X%, BYVAL Y%)
DECLARE SUB G1Mode (BYVAL ModeNr%)
DECLARE SUB G1Write (St$)
DECLARE SUB G1WriteLn (St$)


CONST LEFT = &H1
CONST RIGHT = &H2
CONST MIDDLE = &H4
CONST FALSE = 0
CONST TRUE = NOT 0
	

    DIM MapHand AS BitMap

    MapHand.ptr1 = 3072
    MapHand.screen1 = -7681
    MapHand.ptr2 = 3072
    MapHand.screen2 = -7681
    MapHand.ptr3 = 3072
    MapHand.screen3 = -7681
    MapHand.ptr4 = 3456
    MapHand.screen4 = -8129
    MapHand.ptr5 = 3504
    MapHand.screen5 = -8185
    MapHand.ptr6 = 28086
    MapHand.screen6 = -32768
    MapHand.ptr7 = -4098
    MapHand.screen7 = 0
    MapHand.ptr8 = -4097
    MapHand.screen8 = 0
    MapHand.ptr9 = -1
    MapHand.screen9 = 0
    MapHand.ptr10 = -1
    MapHand.screen10 = 0
    MapHand.ptr11 = 32766
    MapHand.screen11 = -32768
    MapHand.ptr12 = 16380
    MapHand.screen12 = -32767
    MapHand.ptr13 = 4080
    MapHand.screen13 = -8185
    MapHand.ptr14 = 4080
    MapHand.screen14 = -8185
    MapHand.ptr15 = 4080
    MapHand.screen15 = -8185
    MapHand.ptr16 = 4080
    MapHand.screen16 = -8185

' End of LUMPY2.BI include file!

