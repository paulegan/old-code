#include <stdio.h>
#include "simon/manlib.h"

#define WINSIZE 200
#define INFINITY 100

void plotMandel(float xCorn, float yCorn, float size);

void main()
{
 float xCorn,yCorn,size;
 int button=0,mouseX,mouseY;
 
 printf("A program to plot the Mandelbrot Set in the complex plane. \n");
 printf("The top-lefthand corner of the plane is given by complex number (a+bi). \n");
 printf("The size of the plane is given by non-zero s. \n");
 printf("Input the real numbers a b and s (-2 -1.25 2.5 are a good start) : ");

 scanf("%f %f %f",&xCorn,&yCorn,&size);
 
 makewindow(WINSIZE);
 
 while(button != 2){
  plotMandel(xCorn,yCorn,size);

  printf("\nFinished plotting Mandelbrot Set \n");
  printf("Click middle mouse button to exit \n");
  printf("Click left mouse button to enlarge region currently pointed at \n");
  printf("Click right mouse button to reduce magnification \n");
 
  button=getmouse(&mouseX,&mouseY);
  if (button==1){
    xCorn+=size*(((float)mouseX/WINSIZE)-0.05);
    yCorn+=size*(((float)mouseY/WINSIZE)-0.05);
    size/=10;
  }
  if (button==3){
    xCorn+=size*(((float)mouseX/WINSIZE)-5);
    yCorn+=size*(((float)mouseY/WINSIZE)-5);
    size*=10;
  }
  printf("Plotting from (%f + %fi), size %f \n",xCorn,yCorn,size);
  
 }/* while */
 
 
}/* main */


void plotMandel(float xCorn, float yCorn, float size)
{
 struct complex{float a,b;} z,c;
 float zaSq,zbSq;
 int j,k,count;

 for(j=1;j<WINSIZE;j++)
   for(k=1;k<WINSIZE;k++){
     count=0;
     c.a=xCorn+j*size/WINSIZE;
     c.b=yCorn+k*size/WINSIZE;
     z.a=z.b=0;
     zaSq=zbSq=0;
     while((count<INFINITY) && ((zaSq+zbSq)<=4)){
       count++;
       zaSq=(z.a*z.a);
       zbSq=(z.b*z.b);
       z.b=(2 * z.a * z.b)+c.b;
       z.a=zaSq-zbSq+c.a;
     }
     
     if (count<100) drawpixel(j,k,(count/5)+1);
     else drawpixel(j,k,0);
     
   } /* for */

}/* plotMandel */
