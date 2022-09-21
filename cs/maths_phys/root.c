
#include <stdio.h>
#include <math.h>

#define f(x) 		(sin((x)*2/5)-(x)+1)
#define fd(x)		(2/5*sin((x)*2/5)-1)
#define PRECISION	0.000001

double rootByBisection(double a,double b,long *iterations);
double rootByNewton(double x, long *iterations);
void   tabulateFunction();
void   getEstimates(double *a, double *b, double *x);

void main()
{
 double a,b,x;
 long 	iterations;
 
 tabulateFunction();
 
 getEstimates(&a,&b,&x);
 
 printf("Calculating root over interval [%g,%g] with initial estimate %g\n",a,b,x);
 printf("Root using bisection method: %.5f ",rootByBisection(a,b,&iterations));
 printf("  Iterations = %ld \n",iterations);
 printf("Root using Newton method: %.5f ",rootByNewton(x,&iterations));
 printf("  Iterations = %ld \n",iterations);
 
} /* main */

void getEstimates(double *a, double *b, double *x)
{
 do{
  printf("Please enter the initial interval for the bisection method: ");
  scanf("%lf %lf",a,b);
  if (f(*a)*f(*b) > 0) 
   printf("Interval must include the root!\n");
 }while(f(*a)*f(*b) > 0);
 
 printf("Please enter the initial estimate for Newton method: ");
 scanf("%lf",x);
 
}/* getEstimates */

void tabulateFunction()
{
 double a,b,x;
 double tabStep;
 
 do{
  printf("Please enter the interval for tabulation: ");
  scanf("%lf %lf",&a,&b);
  printf("Please specify the tabulation step: ");
  scanf("%lf",&tabStep);

  printf("Tabulating [%g,%g] with step %g \n",a,b,tabStep);
  for(x=a; x<=b; x+=tabStep)
   printf("x=%g f(x)=%.5f\n",x,f(x));
  
  getchar();
  printf("Tabulate again [y/n] ? ");
 }while(getchar()!='n');
}/* tabulateFunction */

double rootByBisection(double a,double b, long *iterations)
{
 double c;

 *iterations = 0;
 do{
  c = (a+b)/2;
  if (f(a)*f(c) < 0) b=c;
  		else a=c;
  *iterations++;
 }while((b-a)/2 > PRECISION);
 
 return(c);
} /* rootByBisection */
 
double rootByNewton(double x, long *iterations)
{
 double lastx;

 *iterations = 0; 
 do{
  lastx = x;
  x = lastx-(f(x)/fd(x));
  *iterations++;
 }while(fabs(x-lastx) > PRECISION);
 
 return(x);
}/* rootByDouble */
