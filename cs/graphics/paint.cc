
// paint.cc

// lumpy@cs.ucc.ie	15.1.1998

// Paul Egan	94312974

#include <misc.h>
#include <Window.h>
#include <DrawArea.h>
#include <PixMap.h>
#include <MenuBar.h>
#include <ButtonBar.h>
#include <ColourBar.h>
#include <OptionBox.h>
#include <pixmapData.h>

// Instantiate top level objects:

window		theWindow("Paint",600,400);
MenuBar		menuBar(&theWindow);
ColourBar	colourBar(&theWindow);
ButtonBar	buttonBar(&theWindow,colourBar.extent().topRight().yValue()+1,menuBar.extent().bottomLeft().yValue()-1);
DrawArea	drawArea(&theWindow,Rectangle(buttonBar.extent().topRight().xValue()+1,colourBar.extent().topRight().yValue()+1,theWindow.canvasWidth()-1,menuBar.extent().bottomLeft().yValue()-1));
OptionBox	noOptions;
OptionBox	fillOptions;
OptionBox	lineWidths;

// Define callbacks:

void exitPaint()		{ exit(0); }
void clearDrawArea()		{ drawArea.clear(); }
void selectColWhite()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(White); drawArea.setBackground(White); } else { colourBar.setForeground(White); drawArea.setForeground(White); } }
void selectColBlack()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Black); drawArea.setBackground(Black);  } else { colourBar.setForeground(Black); drawArea.setForeground(Black);} }
void selectColLightGrey()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Grey85); drawArea.setBackground(Grey85); } else { colourBar.setForeground(Grey85); drawArea.setForeground(Grey85); } }
void selectColDarkGrey()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Grey40); drawArea.setBackground(Grey40);  } else { colourBar.setForeground(Grey40); drawArea.setForeground(Grey40);} }
void selectColRed()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Red); drawArea.setBackground(Red); } else { colourBar.setForeground(Red); drawArea.setForeground(Red); } }
void selectColDarkRed()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkRed); drawArea.setBackground(DarkRed); } else { colourBar.setForeground(DarkRed); drawArea.setForeground(DarkRed); } }
void selectColBlue()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Blue); drawArea.setBackground(Blue); } else { colourBar.setForeground(Blue); drawArea.setForeground(Blue); } }
void selectColDarkBlue()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkBlue); drawArea.setBackground(DarkBlue); } else { colourBar.setForeground(DarkBlue); drawArea.setForeground(DarkBlue); } }
void selectColGreen()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Green); drawArea.setBackground(Green); } else { colourBar.setForeground(Green); drawArea.setForeground(Green); } }
void selectColDarkGreen()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkGreen); drawArea.setBackground(DarkGreen); } else { colourBar.setForeground(DarkGreen); drawArea.setForeground(DarkGreen); } }
void selectColYellow()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Yellow); drawArea.setBackground(Yellow); } else { colourBar.setForeground(Yellow); drawArea.setForeground(Yellow); } }
void selectColDarkYellow()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkYellow); drawArea.setBackground(DarkYellow); } else { colourBar.setForeground(DarkYellow); drawArea.setForeground(DarkYellow); } }
void selectColCyan()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Cyan); drawArea.setBackground(Cyan); } else { colourBar.setForeground(Cyan); drawArea.setForeground(Cyan); } }
void selectColDarkCyan()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkCyan); drawArea.setBackground(DarkCyan); } else { colourBar.setForeground(DarkCyan); drawArea.setForeground(DarkCyan); } }
void selectColMagenta()		{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(Magenta); drawArea.setBackground(Magenta); } else { colourBar.setForeground(Magenta); drawArea.setForeground(Magenta); } }
void selectColDarkMagenta()	{ if (theWindow.lastButton()==RightButton) { colourBar.setBackground(DarkMagenta); drawArea.setBackground(DarkMagenta); } else { colourBar.setForeground(DarkMagenta); drawArea.setForeground(DarkMagenta); } }
void lineDrawMode()		{ buttonBar.setOptionBoxTo(&lineWidths); drawArea.setMode(LineMode); }
void rectDrawMode()		{ buttonBar.setOptionBoxTo(&fillOptions); drawArea.setMode(RectangleMode); }
void elpsDrawMode()		{ buttonBar.setOptionBoxTo(&fillOptions); drawArea.setMode(EllipseMode); }
void polyDrawMode()		{ buttonBar.setOptionBoxTo(&fillOptions); drawArea.setMode(PolygonMode); }
void nullMode()			{ buttonBar.setOptionBoxTo(&noOptions); }
void selectLineWidth1()		{ drawArea.setLineWidth(1); }
void selectLineWidth2()		{ drawArea.setLineWidth(2); }
void selectLineWidth3()		{ drawArea.setLineWidth(3); }
void selectLineWidth4()		{ drawArea.setLineWidth(4); }
void selectLineWidth5()		{ drawArea.setLineWidth(5); }
void selectNoFill()		{ drawArea.setFillStyle(NoFill); }
void selectFullFillWithB()	{ drawArea.setFillStyle(FillWithBorder); }
void selectFullFill()		{ drawArea.setFillStyle(FillWithoutBorder); }


int main()
{

 // Build & display menu bar:
 Menu  fileMenu("File");
 fileMenu.addItem(MenuItem("New",&clearDrawArea,true));
 fileMenu.addItem(MenuItem("Open...",NULL,false));
 fileMenu.addItem(MenuItem("Save",NULL,false));
 fileMenu.addItem(MenuItem("Save As...",NULL,false));
 fileMenu.addItem(MenuItem("",NULL,false));
 fileMenu.addItem(MenuItem("Print Preview",NULL,false));
 fileMenu.addItem(MenuItem("Page Setup...",NULL,false));
 fileMenu.addItem(MenuItem("Print...",NULL,false));
 fileMenu.addItem(MenuItem("",NULL,false));
 fileMenu.addItem(MenuItem("Exit",&exitPaint,true));
 menuBar.addItem(fileMenu);
 Menu  editMenu("Edit");
 editMenu.addItem(MenuItem("Undo",NULL,false));
 editMenu.addItem(MenuItem("Repeat",NULL,false));
 editMenu.addItem(MenuItem("",NULL,false));
 editMenu.addItem(MenuItem("Cut",NULL,false));
 editMenu.addItem(MenuItem("Copy",NULL,false));
 editMenu.addItem(MenuItem("Paste",NULL,false));
 editMenu.addItem(MenuItem("Clear Selection",NULL,false));
 editMenu.addItem(MenuItem("Select All",NULL,false));
 editMenu.addItem(MenuItem("",NULL,false));
 editMenu.addItem(MenuItem("Copy To...",NULL,false));
 editMenu.addItem(MenuItem("Paste From...",NULL,false));
 menuBar.addItem(editMenu);
 Menu  viewMenu("View");
 viewMenu.addItem(MenuItem("Tool Box",NULL,false));
 viewMenu.addItem(MenuItem("Color Box",NULL,false));
 viewMenu.addItem(MenuItem("Status Bar",NULL,false));
 viewMenu.addItem(MenuItem("",NULL,false));
 viewMenu.addItem(MenuItem("Zoom",NULL,false));
 menuBar.addItem(viewMenu);
 Menu  optionsMenu("Options");
 optionsMenu.addItem(MenuItem("Edit Colors...",NULL,false));
 optionsMenu.addItem(MenuItem("Get Colors...",NULL,false));
 optionsMenu.addItem(MenuItem("Save Colors...",NULL,false));
 optionsMenu.addItem(MenuItem("Draw Opaque",NULL,false));
 menuBar.addItem(optionsMenu);
 Menu  imageMenu("Image");
 imageMenu.addItem(MenuItem("Flip/Rotate...",NULL,false));
 imageMenu.addItem(MenuItem("Stretch/Skew...",NULL,false));
 imageMenu.addItem(MenuItem("Invert Colors",NULL,false));
 imageMenu.addItem(MenuItem("Attributes...",NULL,false));
 imageMenu.addItem(MenuItem("Clear Image",&clearDrawArea,true));
 menuBar.addItem(imageMenu);
 Menu  helpMenu("Help");
 helpMenu.addItem(MenuItem("Help Topics",NULL,false));
 helpMenu.addItem(MenuItem("",NULL,false));
 helpMenu.addItem(MenuItem("About Paint",NULL,false));
 menuBar.addItem(helpMenu);
 menuBar.display();

 // Build & display colour bar:
 colourBar.addItem(ColourItem(White,&selectColWhite,true));
 colourBar.addItem(ColourItem(Black,&selectColBlack,true));
 colourBar.addItem(ColourItem(Grey85,&selectColLightGrey,true));
 colourBar.addItem(ColourItem(Grey40,&selectColDarkGrey,true));
 colourBar.addItem(ColourItem(Red,&selectColRed,true));
 colourBar.addItem(ColourItem(DarkRed,&selectColDarkRed,true));
 colourBar.addItem(ColourItem(Yellow,&selectColYellow,true));
 colourBar.addItem(ColourItem(DarkYellow,&selectColDarkYellow,true));
 colourBar.addItem(ColourItem(Green,&selectColGreen,true));
 colourBar.addItem(ColourItem(DarkGreen,&selectColDarkGreen,true));
 colourBar.addItem(ColourItem(Cyan,&selectColCyan,true));
 colourBar.addItem(ColourItem(DarkCyan,&selectColDarkCyan,true));
 colourBar.addItem(ColourItem(Blue,&selectColBlue,true));
 colourBar.addItem(ColourItem(DarkBlue,&selectColDarkBlue,true));
 colourBar.addItem(ColourItem(Magenta,&selectColMagenta,true));
 colourBar.addItem(ColourItem(DarkMagenta,&selectColDarkMagenta,true));
 colourBar.display();

 // Build & display button bar:
 buttonBar.addItem(Button(PixMap(select1ButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(select2ButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(eraserButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(fillButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(dropperButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(zoomButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(pencilButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(paintbrushButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(sprayCanButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(fontButtonData,18,18),&nullMode,true));
 buttonBar.addItem(Button(PixMap(lineButtonData,18,18),&lineDrawMode,true));
 buttonBar.addItem(Button(PixMap(line2ButtonData,18,18),&lineDrawMode,true));
 buttonBar.addItem(Button(PixMap(rectButtonData,18,18),&rectDrawMode,true));
 buttonBar.addItem(Button(PixMap(polygonButtonData,18,18),&polyDrawMode,true));
 buttonBar.addItem(Button(PixMap(ellipseButtonData,18,18),&elpsDrawMode,true));
 buttonBar.addItem(Button(PixMap(roundRectButtonData,18,18),&rectDrawMode,true));
 buttonBar.display();

 // Build option boxes:
 lineWidths.addItem(OptionBoxItem(PixMap(lineWidthOpt1,30,8),&selectLineWidth1,true));
 lineWidths.addItem(OptionBoxItem(PixMap(lineWidthOpt2,30,8),&selectLineWidth2,true));
 lineWidths.addItem(OptionBoxItem(PixMap(lineWidthOpt3,30,8),&selectLineWidth3,true));
 lineWidths.addItem(OptionBoxItem(PixMap(lineWidthOpt4,30,8),&selectLineWidth4,true));
 lineWidths.addItem(OptionBoxItem(PixMap(lineWidthOpt5,30,8),&selectLineWidth5,true));
 
 fillOptions.addItem(OptionBoxItem(PixMap(noFill,30,13),&selectNoFill,true));
 fillOptions.addItem(OptionBoxItem(PixMap(fullFillWithB,30,13),&selectFullFillWithB,true));
 fillOptions.addItem(OptionBoxItem(PixMap(fullFill,30,13),&selectFullFill,true));

 // Whole program involves dealing with a mouse button event:
 while(TRUE){
   theWindow.waitForMouseDown();
   if ( menuBar.extent().contains(theWindow.mouseLocation()) )
     menuBar.dealWithMouseDown();
   else if ( buttonBar.extent().contains(theWindow.mouseLocation()) )
     buttonBar.dealWithMouseDown();
   else if ( colourBar.extent().contains(theWindow.mouseLocation()) )
     colourBar.dealWithMouseDown();
   else if ( drawArea.extent().contains(theWindow.mouseLocation()) )
     drawArea.dealWithMouseDown();
 }
}// main
