#include <stdio.h>
#include <strings.h>

#define LineLength	70
#define ANSIreverse 	{ ' ',' ',' ',27,'[','7','m',0 }
#define ANSIrevbold	{ ' ',' ',' ',27,'[','1',';','7','m',0 }
#define ANSInormal 	{ 27,'[','0','m',0 }
#define ANSImoveTL	{ 27,'[','0',';','0','H',0 }
#define ANSImoveTR	{ 27,'[','0',';','-','1','0','H',0 }
#define ANSIsaveCsr	{ 27,'[','s',0 }
#define ANSIrestCsr	{ 27,'[','u',0 }
#define XTermCode	{ 27,'[',']','2',';',0 }


void main(int argc, char *argv[])
{
 char reverse[] = ANSIreverse;
 char normal[] = ANSInormal;
 char revBold[] = ANSIrevbold;
 char move2TL[] = ANSImoveTL;
 char move2TR[] = ANSImoveTR;
 char saveCsr[] = ANSIsaveCsr;
 char restCsr[] = ANSIrestCsr;
 char xt[] = XTermCode;

 printf("%s test %c",xt,7);

}/* main */

