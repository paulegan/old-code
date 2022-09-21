
/*	snow.c		- Simulates analog snow		*/

/*	lumpy@cs.ucc.ie		2.3.1998		*/


#include <xstuff.h>

#define WWidth			500
#define WHeight			500
#define MaxPoints		400


int	label[MaxPoints], tour[MaxPoints];
int	numPoints, tourLen;
double	x[MaxPoints],y[MaxPoints];
double	xMax,xMin,yMax,yMin;
double	ratio,xRatio,yRatio;
char	ch,tempStr[40];


char getWord(char *word)
{
 char *c = word;
 char last;

 while ( *c = getchar() ){
   if ( (*c==' ') || (*c=='\n') || (*c==EOF) ) break;
   c++;
 }
 last = *c;
 *c = '\0';

 return(last);
}/* getWord */


void drawTour(int col)
{
 static int prevTour[MaxPoints], prevLen=0;
 int i=0,p1,p2,t;
 int *curTour, l;

 XSetFunction(display,gc,(col==2)?GXxor:GXset);
 XSetForeground(display,gc,colours[col].pixel);

 for (t=0; t<2; t++){
   if (t){
     curTour = tour;
     l = tourLen;
   }else{
     curTour = prevTour;
     l = prevLen;
   }
   if (l){
     for (p1=0;p1<numPoints;p1++) if ( curTour[0] == label[p1] ) break;
     for (i=1;i<l;i++){
       for (p2=0;p2<numPoints;p2++) if ( curTour[i] == label[p2] ) break;
       XDrawLine(display,win,gc,
	    ratio*(x[p1]+fabs(xMin)+((xMax-xMin)*0.05)),
	    WHeight-(ratio*(y[p1]+fabs(yMin)+((yMax-yMin)*0.05))),
	    ratio*(x[p2]+fabs(xMin)+((xMax-xMin)*0.05)),
	    WHeight-(ratio*(y[p2]+fabs(yMin)+((yMax-yMin)*0.05))));
       p1 = p2;
     }
   }
 }


 for (t=0; t<tourLen; t++)
   prevTour[t] = tour[t];
 prevLen = tourLen;

 XFlush(display);
}/* drawTour */


void plotPoints()
{
 int i,xVal,yVal;

 XSetForeground(display,gc,colours[4].pixel);
 XDrawLine(display,win,gc,ratio*(fabs(xMin)+((xMax-xMin)*0.05)),0,ratio*(fabs(xMin)+((xMax-xMin)*0.05)),WHeight);
 XDrawLine(display,win,gc,0,WHeight-(ratio*(fabs(yMin)+((yMax-yMin)*0.05))),WWidth,WHeight-(ratio*(fabs(yMin)+((yMax-yMin)*0.05))));

 XSetForeground(display,gc,colours[0].pixel); 
 for (i=0; i<numPoints; i++){
   xVal = (int)(ratio*(x[i]+fabs(xMin)+((xMax-xMin)*0.05)));
   yVal = (int)(WHeight-(ratio*(y[i]+fabs(yMin)+((yMax-yMin)*0.05))));
   XDrawPoint(display,win,gc,xVal,yVal);
   XDrawArc(display,win,gc,xVal-3,yVal-3,6,6,0,23040);
   /* sprintf(tempStr,"%d: (%.2f,%.2f)",label[i],x[i],y[i]); */
   sprintf(tempStr,"%d",label[i]);
   XPrint(tempStr,xVal+6,yVal-4); 
 }
}/* plotPoints */


int main(int argc, char **argv)
{
 FILE *fd;
 int i,n=0;
 
 if ((fd = fopen(argv[1],"r"))==NULL)
   printf("Couldn't open %s!\n",argv[1]);
 while (!feof(fd)){
  fscanf(fd," %d %lf %lf",&label[n],&x[n],&y[n]);
  n++;
 }
 fclose(fd);
 numPoints = n-1;

 xMin = xMax = x[0];
 yMin = yMax = y[0];
 for (i=0; i<numPoints; i++){
  if (x[i]>xMax) xMax=x[i];
  if (x[i]<xMin) xMin=x[i];
  if (y[i]>yMax) yMax=y[i];
  if (y[i]<yMin) yMin=y[i];
 }

 xRatio = WWidth/((xMax-xMin)*1.1);
 yRatio = WHeight/((yMax-yMin)*1.1);
 ratio = (xRatio>yRatio)?xRatio:yRatio;

 openWindow("TSP",WWidth,WHeight);

 plotPoints();

 while ( ch!=EOF ){
   tourLen=0;
   while ( ch=getWord(tempStr) ){
     if (strlen(tempStr)) sscanf(tempStr,"%d",&tour[tourLen++]);
     if ( (ch=='\n') || (ch==EOF) ) break;
   }
   if (tourLen) drawTour(2);
 }

 drawTour(1);
 
 waitForMouseClick();
 closeWindow();

 return(0);
}/* main */

