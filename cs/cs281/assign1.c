#include <stdio.h>
#include <sys/time.h>

#define TABLESIZE 	523
#define EMPTY     	-1
#define LOOPSFORACURACY 1000

enum strategy {linearProbe,addHashRehash,secondaryHash};

typedef struct{
 unsigned int numProbesForFound;
 unsigned int numProbesForNotFound;
}probeInfoType;

void initializeTable();
void rehash(int key, int *pos, enum strategy s);
void insertValues(int load, enum strategy s);
void checkForValues(int load, enum strategy s, probeInfoType *probeInfo);

int table[TABLESIZE];


void main(){
 probeInfoType 	probeInfo;
 int   		i;
 int   		randomSeed;
 float 		loadFactor;
 
 probeInfo.numProbesForFound=0;
 probeInfo.numProbesForNotFound=0;
 loadFactor=0.5;
 
 for(i=0;i<LOOPSFORACURACY;i++){
  initializeTable();

  randomSeed = (int)time(NULL);
  srandom(randomSeed);
  insertValues(loadFactor*TABLESIZE,linearProbe);
 
  srandom(randomSeed);
  checkForValues(loadFactor*TABLESIZE,linearProbe,&probeInfo); 
  checkForValues(loadFactor*TABLESIZE,linearProbe,&probeInfo);
 }/* for */
 
 printf("numFound=%d probes=%d \n",(int)(loadFactor*TABLESIZE),
		probeInfo.numProbesForFound/LOOPSFORACURACY);
 printf("numNotFound=%d probes=%d \n",(int)(loadFactor*TABLESIZE),
		probeInfo.numProbesForNotFound/LOOPSFORACURACY);

}/* main */



void initializeTable()
{
 int i;

 for (i=0;i<TABLESIZE;i++)
    table[i]=EMPTY;
 
}/* initialize */


void rehash(int key, int *pos, enum strategy s)
{
 switch(s){
   case linearProbe: 
    if (--(*pos)<0) (*pos)=TABLESIZE-1;
   break;
   case addHashRehash: 
    (*pos)=((*pos)+key)%TABLESIZE;
   break;
   case secondaryHash: 
    (*pos)=1+(key%(TABLESIZE-2));
   break;
 }

}/* rehash */


void insertValues(int load, enum strategy s)
{
 int i,position,key;
 
 for (i=0;i<load;i++){
   key=(int)random();
   position=key%TABLESIZE;	

   while (table[position]!=EMPTY)
     rehash(key,&position,s);
   
   table[position]=key; 
     
 }  	/* for */
  
}/* insertValues */


void checkForValues(int load, enum strategy s, probeInfoType *probeInfo)
{
 int i,position,key;
 int numOfProbes;
 
 for (i=0;i<load;i++){
   key=(int)random();
   position=key%TABLESIZE;
   numOfProbes=1;
   
   do{
     if (table[position]!=EMPTY && table[position]!=key){
       rehash(key,&position,s);
       numOfProbes++;
     }
     if (table[position]==key)
       probeInfo->numProbesForFound+=numOfProbes;
     if (table[position]==EMPTY)
       probeInfo->numProbesForNotFound+=numOfProbes;
       
   }while (table[position]!=EMPTY && table[position]!=key);
     
 }  	/* for */

}/* insertValues */


