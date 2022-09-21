
// misc.cc

#include <misc.h>
#include <srgp.h>

void loadColours()
{
 SRGP_loadCommonColor(Grey40,"grey40");
 SRGP_loadCommonColor(Grey85,"grey85");
 SRGP_loadCommonColor(Grey95,"grey95");
 SRGP_loadCommonColor(Red,"red");
 SRGP_loadCommonColor(Green,"green");
 SRGP_loadCommonColor(Blue,"blue");
 SRGP_loadCommonColor(Yellow,"yellow");
 SRGP_loadCommonColor(Cyan,"cyan");
 SRGP_loadCommonColor(Magenta,"magenta");
 SRGP_loadCommonColor(DarkRed,"DarkRed");
 SRGP_loadCommonColor(DarkGreen,"DarkGreen");
 SRGP_loadCommonColor(DarkBlue,"DarkBlue");
 SRGP_loadCommonColor(DarkYellow,"yellow4");
 SRGP_loadCommonColor(DarkCyan,"DarkCyan");
 SRGP_loadCommonColor(DarkMagenta,"DarkMagenta");
}// loadColours

void loadFonts()
{
 SRGP_loadFont(HelvBold16,"-*-helvetica-bold-r-normal-*-16-*-*-*-*-*-*-*");
}// loadFonts


Rectangle textExtent(String text, FontID f)
{
 int width,ascent,descent;
 
 SRGP_setFont(f);
 SRGP_inquireTextExtent(text,&width,&ascent,&descent);
 return(Rectangle(0,-descent,width,ascent));
}// textExtent




