/* 
 * mandel.c 
 * A program for graphically/interactively exploring the Mandelbrot Set.
 */
#include "manclientlib.h"

int colorfactor;         /*used to color code values 1..INFINITY with NRCOLORS*/
unsigned int start,finish;


/*
 * Given complex region with top-left hand corner (acorn,bcorn) width size, 
 * convert the window pixel coordinate (x,y) to the complex coordinate 
 * (*cx,*cy) that it represents. 
 */
void pixel_to_complex(long double acorn,long double bcorn, 
                      long double size, 
                      unsigned int x, unsigned int y,
                      long double *cx, long double *cy)
{
    *cx= acorn+(x*size/WINSIZE);
    *cy= bcorn+(y*size/WINSIZE);
}/*pixel_to_complex*/

/*
 * Draw the portion of the Mandelbrot set in the complex-plane, with top-left hand 
 * corner (acorn,bcorn) and bottom right hand-corner (acorn+size,bcorn+size).
 */
void draw_mandel(long double acorn, long double bcorn, 
                 long double size)
{
    int j,k;                                  /*current pixel coordinate and, */
    long double  ca, cb,                 /*the complex value c it `represents'*/
                 zx,zy,                         /*complex z from f(z) = z^2+c */
                 zxs,zys,                             /*complex zs = square(z)*/
                 xtemp;
    int count;                                  /*count of times f(z) iterated*/ 


    for (j=start; j<finish; j++){
        for (k=1; k<=WINSIZE; k++)       /*consider each pixel value in window*/
        {

                             /*determine the complex c for this pixel position*/
            pixel_to_complex(acorn,bcorn,size,j,k,&ca,&cb);

            zx=0; zy=0;                   /*initialize complex variable z to 0*/
            zxs=0; zys=0;                            /*square(z) = zs = (0+i0)*/    
                 /*iterate f(z)=z+c, substituting f(z) for z on each iteration*/ 
            count=0;
            while ((count<INFINITY)           /*have not reached infinity and,*/ 
                  &&(zxs+zys<=4))        /*not yet known whether f(z) diverges*/ 
            {
                /*given zs and c, calculate z=z^2+c*/
                xtemp = zx;
                zx= zxs-zys  +ca; 
                zy= (2*xtemp*zy)+cb;

                /*given the new z, calculate z^2*/
                zxs=(zx*zx);    zys=(zy*zy);

                count= count+1;
            } 
        
            if (count==INFINITY) 
                /*we iterated f(z) an `infinite' number of times without
                  diverging, and so we have a bounded orbit, i.e. this 
                  complex c is in the Mandelbrot set*/
                drawpixel(j,k,COLORBLACK);
            else 
                /*we iterated count times, and ended up with |f(z)|>2 
                  and can conclude that it diverges to infinity (unbounded
                  orbit), i.e., c is not in Mandelbrot set. Color code the
                  nr of iterations made to represent the rate of diveregence*/
                drawpixel(j,k,(count/colorfactor)+1);
        }
    }
}/*draw_mandel*/


/* 
 * Request the coordinates for the initial complex plane to be studied in 
 * terms of top-left hand corner coordinate (acorn,bcorn) and width (height) 
 * size. Draw that/any part of the mandelbrot set found in this region. 
 * Once drawn, the region may be sucessively enlarged/reduced using the
 * mouse buttons. 
 */
void main(){
    long double acorn,            /*Complex plane coordinates for the top-left*/
                bcorn,                 /*corner of the region we intend to map*/
                size;                        /*region is a square of this size*/
    int button;                                         /*mouse button pressed*/
    unsigned int x,y;                /*and corresponding coordinate of pointer*/


    window_connect();
/*
    printf("Enter acorner, bcorned and size\n");
    scanf("%lf %lf %lf", &acorn, &bcorn, &size); 
*/
    acorn=-2;  bcorn=-1.25;  size=2.5;

    printf("Enter start and finish \n");
    scanf("%u %u", &start, &finish);

    colorfactor=5;

    printf("Plotting from (%lf + %lfi), size %lf\n", acorn,bcorn,size);
    draw_mandel(acorn,bcorn,size);
}/*main*/

