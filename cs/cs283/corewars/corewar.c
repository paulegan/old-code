
/*  			    		CS 283/5 Assignment 2						*/
/* 				      Submission Date: 2/2/1996						*/

/* 		Written by Paul Egan (94312974) and Michael Cronin (94337454).				*/

/* corewar.c - A MARS simulator. A program to run two redcode warriors in battle against eachother	*/
/*	until a stalemate is reached or an invalid instuction is attemted to execute			*/

/* Note - This program is based on the '92 icws spec and not on the '84 spec as decribed in the 	*/
/*	introductory sheet for this assignment.  Notable differences are a new instruction encoding	*/
/*	format, an extra addressing mode (predecrement indirect) and subtle differences between 	*/
/*	instructions (jmn [jump if not zero] instead of jmg [jmp if greater than zero] and djn 		*/
/*	[decrement, jump if not zero] instead of djz [decrement, jump if zero]).			*/
/*	The reason for basing this simulator on the later spec was to make it compatable with the	*/
/*	provided assembler, and therefore give it the ability to run more complicated warriors.		*/


#include <stdio.h>						/* Include standard header files	*/
#include <stdlib.h>
#include <sys/time.h>
#include "bool.h"						/* Include file for boolean data types	*/
#include "xstuff.h"						/* Function prototypes and related	*/
								/*  declarations for X interface	*/

#define CORESIZE  	8000					/* The size of the core array		*/
#define TIMEOUT	  	20000					/* Number of iterations before 		*/
								/*   stalemate is declared		*/
#define LOCSPERLINE	(CORESIZE/WINWIDTH)			/* Number of core positions represented	*/
								/*   by each line			*/

enum opCodes {dat,mov,add,sub,jmp,jmz,jmn,djn,cmp};		/* The 9 operation codes		*/
enum modes   {immediate,direct,indirect,indirect_dec};		/* The 4 addressing modes		*/

typedef struct{							/* Structure for holding instruction	*/
  enum opCodes opCode;						/*  information				*/
  enum modes   modeA;
  unsigned int operandA;
  enum modes   modeB;
  unsigned int operandB;
}instruction;

typedef struct{							/* Structure for holding information	*/
  unsigned int PC;						/*   on each warrior			*/
  enum colours brightColour;
  enum colours darkColour;
  bool hasLost;
}warriorType;

instruction core[CORESIZE];					/* The core array "Suprise suprise!!	*/
windowType  mainWindow={NULL,NULL,NULL,"fixed"};		/* The main window info			*/

bool validInstruction(int addr);				/* Function prototypes			*/
void initialize(warriorType warriors[]);
void runBattle(warriorType warrior[]);
void exec(warriorType *warrior);
void openFile(int w, FILE **wFile);
void readInWarriors(warriorType warriors[]);
bool usersChoice(warriorType warrior[]);


void main()
{
 bool	     notFinished=TRUE;
 warriorType warriors[2]; 
 
 initX();							/* Open connection to display		*/
 
 do{
  initialize(warriors);						/* Initialize core and warriors		*/
  readInWarriors(warriors);					/* Read in the 2 warriors		*/
  makeWindow(WINWIDTH,WINHEIGHT,&mainWindow);			/* Create the main window		*/
 
  runBattle(warriors);						/* Run the battle			*/
 
  notFinished = usersChoice(warriors);				/* Display final dialog box and return	*/
  								/*   if user selected the exit button	*/
  closeWindow(mainWindow);
 }while(notFinished);
 
 closeX();
  
}/* main */

void initialize(warriorType warriors[2])			/* Initializes the core and warriors	*/
{
 int x;
  
 for(x=0;x<CORESIZE;x++){
  core[x].opCode = 0;
  core[x].modeA = 0;
  core[x].operandA = 0;
  core[x].modeB = 0;
  core[x].operandB = 0;
 }

 warriors[0].brightColour=blue;
 warriors[0].darkColour=darkBlue;
 warriors[0].hasLost=FALSE; 
 warriors[1].brightColour=red;
 warriors[1].darkColour=darkRed;
 warriors[1].hasLost=FALSE;
 
}/* initialize */

bool validInstruction(int addr)					/* Function that returns true or false	*/
{								/*   whether or not instuction at addr	*/
 								/*   is valid				*/
 if (core[addr].opCode<mov || core[addr].opCode>cmp)
   return(FALSE);
 if (core[addr].modeA>indirect_dec || core[addr].modeB>indirect_dec)
   return(FALSE);
 if (core[addr].opCode>=mov && core[addr].opCode<=sub && core[addr].modeB==immediate)
   return(FALSE);
 if (core[addr].opCode>=jmp && core[addr].opCode<=djn && core[addr].modeA==immediate)
   return(FALSE);
 if (core[addr].opCode==djn && core[addr].modeB==immediate)
   return(FALSE);

 return(TRUE);
}/* validInstruction */

void runBattle(warriorType warrior[])				/* Keeps alternatively the 2 warriors	*/
{								/*   instruction until an invalid	*/
 int counter=0;							/*   instruction is attempted to be	*/
 bool notFinished=TRUE;						/*   executed or a stalemate is reached	*/

 do{
  if (validInstruction(warrior[0].PC))
   exec(&warrior[0]);
  else
   warrior[0].hasLost = TRUE;

  if (validInstruction(warrior[1].PC))
   exec(&warrior[1]);
  else
   warrior[1].hasLost = TRUE;
 
  if (warrior[0].hasLost || warrior[1].hasLost || ++counter>TIMEOUT)
   notFinished = FALSE;
 
 }while(notFinished);
   
}/* runBattle */


void exec(warriorType *warrior)					/* Execute the next instruction for	*/
{								/*   warrior				*/
 unsigned int addressA;
 unsigned int addressB;
 instruction  contentsA;
 instruction  contentsB;
 enum opCodes operation;
 								/* Indicate the last pos that the 	*/
 drawPos((warrior->PC)/LOCSPERLINE,warrior->darkColour,mainWindow);		/* warrior held		*/

 switch(core[warrior->PC].modeA){				/* Determine the mode of argument A	*/
  case immediate:						/*   and set the contents of A and 	*/
   contentsA.opCode = dat;					/*   address of A based on this 	*/
   contentsA.modeA = immediate;
   contentsA.operandA = 0;
   contentsA.modeB = immediate;
   contentsA.operandB = core[warrior->PC].operandA;
  break;
  case direct: 
   addressA = ((warrior->PC)+core[warrior->PC].operandA) % CORESIZE;
   contentsA = core[addressA];
  break;
  case indirect:
   addressA = ((warrior->PC)+core[warrior->PC].operandA) % CORESIZE;
   addressA = (addressA+core[addressA].operandB) % CORESIZE;
   contentsA = core[addressA];
  break;
  case indirect_dec:
   addressA = ((warrior->PC)+core[warrior->PC].operandA) % CORESIZE;
   core[addressA].operandB--;
   addressA = (addressA+core[addressA].operandB) % CORESIZE;
   contentsA = core[addressA];
  break;
 }
 
 switch(core[warrior->PC].modeB){				/* Determine the mode of argument B	*/
  case immediate:						/*   and set the contents of B and	*/
   contentsB.opCode = dat;					/*   address of B based on this 	*/
   contentsB.modeA = immediate;
   contentsB.operandA = 0;
   contentsB.modeB = immediate;
   contentsB.operandB = core[warrior->PC].operandB;
  break;
  case direct: 
   addressB = ((warrior->PC)+core[warrior->PC].operandB) % CORESIZE;
   contentsB = core[addressB];
  break;
  case indirect:
   addressB = ((warrior->PC)+core[warrior->PC].operandB) % CORESIZE;
   addressB = (addressB+core[addressB].operandB) % CORESIZE;
   contentsB = core[addressB];
  break;
  case indirect_dec:
   addressB = ((warrior->PC)+core[warrior->PC].operandB) % CORESIZE;
   core[addressB].operandB--;
   addressB = (addressB+core[addressB].operandB) % CORESIZE;
   contentsB = core[addressB];
  break;
 }

 operation = core[warrior->PC].opCode;
 (warrior->PC)=++(warrior->PC)%CORESIZE;			/* Move program counter onto next	*/
 switch(operation){						/*   instruction			*/
  case mov:
   core[addressB] = contentsA;					/* Move contents of A to B and show pos	*/
   drawPos(addressB/LOCSPERLINE,warrior->darkColour,mainWindow);		/* of move		*/
  break;
  case add:							/* Add A to B				*/
   core[addressB].operandB = contentsB.operandB + contentsA.operandB;
  break;
  case sub:							/* Subtract A from B			*/
   core[addressB].operandB = contentsB.operandB - contentsA.operandB;
  break;
  case jmp:							/* Jump to address B			*/
   warrior->PC = addressA;
  break;
  case jmz:							/* Jump if B is zero			*/
   if (contentsB.operandB==0) warrior->PC = addressA;
  break;
  case jmn:							/* Jump if B is not zero		*/
   if (contentsB.operandB!=0) warrior->PC = addressA;
  break;
  case djn:							/* Decrement B, jump if not zero	*/
   if (--core[addressB].operandB!=0) warrior->PC = addressA;
  break;
  case cmp:							/* Skip next instruction if A = B	*/
   if (contentsA.operandB == contentsB.operandB) 
     (warrior->PC)=++(warrior->PC)%CORESIZE;
  break;

 }/* switch */
								/* Indicate new position of warrior	*/
 drawPos((warrior->PC)/LOCSPERLINE,warrior->brightColour,mainWindow);

}/* exec */

void openFile(int w, FILE **wFile)				/* Creates a dialog box inputs a file	*/
{								/*  name and opens that file		*/
 char	     str[40];
 char	    *col[] = {"blue","red"};
 windowType  dialogBox={NULL,NULL,NULL,"9x15"};
 
 makeWindow(403,100,&dialogBox);
 XPrint("Please enter file name",95,20,dialogBox);
 sprintf(str,"for warrior %d [%s]:",w,col[w-1]);
 XPrint(str,100,35,dialogBox);
 drawRectangle(15,50,372,20,black,dialogBox);
 getString(str,20,65,dialogBox);
 
 while((*wFile=fopen(str,"r"))==NULL){
  XPrint("ERROR: Unable to open warrior file",40,85,dialogBox);
  getString(str,20,65,dialogBox);
 }            
 closeWindow(dialogBox);

}/* openFile */

void readInWarriors(warriorType warriors[2])			/* Reads the warriors from files and	*/
{								/*  place them randomly in the core	*/
 struct{
  int  length;
  int  start;
  long position;
 }warriorInfo[2];
 int   argA,argB;
 FILE *warriorFile;
 int   i;
 
 srandom((int)time(NULL));

 openFile(1,&warriorFile);
 
 fscanf(warriorFile,"%i %i",&warriorInfo[0].length,&warriorInfo[0].start);
 warriorInfo[0].position=random()%(CORESIZE-warriorInfo[0].length);	
 warriors[0].PC = warriorInfo[0].position+warriorInfo[0].start;     /* set program counter to start of	*/                                        
 for (i=0;i<warriorInfo[0].length;i++){					/* warriors 1st instruction	*/
     fscanf(warriorFile,"%i %i %i",&core[warriorInfo[0].position].opCode,&argA,&argB);
     core[warriorInfo[0].position].operandA=argA%8192;
     core[warriorInfo[0].position].operandB=argB%8192;
     core[warriorInfo[0].position].modeA=(argA-core[warriorInfo[0].position].operandA)/8192;
     core[warriorInfo[0].position].modeB=(argB-core[warriorInfo[0].position].operandB)/8192;
     warriorInfo[0].position++;
 }              /* for loop */
 fclose(warriorFile);
 
 openFile(2,&warriorFile);
  
 fscanf(warriorFile,"%i %i",&warriorInfo[1].length,&warriorInfo[1].start);
 do{								/* Loop until random pos isn't over	*/ 
  warriorInfo[1].position=random()%(CORESIZE-warriorInfo[1].length);	/* pos of 1st warrior		*/
 }while ((abs(warriorInfo[0].position-warriorInfo[1].position))
 		<(warriorInfo[0].length+warriorInfo[1].length));
 		
 warriors[1].PC = warriorInfo[1].position+warriorInfo[1].start;	      /* set program counter to start of*/                                          
 for (i=0;i<warriorInfo[1].length;i++){					/* warriors 1st instruction	*/
     fscanf(warriorFile,"%i %i %i",&core[warriorInfo[1].position].opCode,&argA,&argB);
     core[warriorInfo[1].position].operandA=argA%8192;
     core[warriorInfo[1].position].operandB=argB%8192;
     core[warriorInfo[1].position].modeA=(argA-core[warriorInfo[1].position].operandA)/8192;
     core[warriorInfo[1].position].modeB=(argB-core[warriorInfo[1].position].operandB)/8192;
     warriorInfo[1].position++;
 }              /* for loop */
 fclose(warriorFile);

}/* readInWarriors */

bool usersChoice(warriorType warrior[])				/* Creates a dialog box with 2 buttons	*/
{								/*   also desplaying result of battle	*/
 windowType  dialogBox={NULL,NULL,NULL,"9x15"};
 bool	     doEnd=FALSE;
 
 makeWindow(403,100,&dialogBox);
 if (warrior[0].hasLost)
  XPrint("Warrior 1 [blue] has lost!",85,30,dialogBox);
 else if (warrior[1].hasLost)
  XPrint("Warrior 2 [red] has lost!",85,30,dialogBox);
 else
  XPrint("A stalemate was reached!",85,30,dialogBox);
  
 drawRectangle(77,54,48,19,black,dialogBox);
 XPrint("EXIT",82,70,dialogBox);
 drawRectangle(235,54,81,19,black,dialogBox);
 XPrint("GO AGAIN",240,70,dialogBox);
 
 if (getButton()==1){						/* If 'exit' button was clicked		*/
  closeWindow(dialogBox);
  return(FALSE);
 }
 else{								/* else 'go again' button clicked upon	*/
  closeWindow(dialogBox);
  return(TRUE);
 }
 
}/* usersChoice */
