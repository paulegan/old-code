
/*				CS284 - Assignment 4					*/
/*				Paul Egan  (94312974)					*/

/* union.c - A program that forms the union of two binary files based on their		*/
/*	descriptor files and ouputs to a third file.					*/


#include <stdio.h>
#include <string.h>
#include "bool.h"

#define FileNameSize 	50
#define MaxNumOfFields	20

void getFileNames(char *inFile1,char *inFile2,char *outFile,int argc,char *argv[]);
void fileUnion(char *inFile1_name,char *inFile2_name,char *outFile_name);
bool recordCompatible(char *inFile1_name,char *inFile2_name);
bool exist(FILE *file, char *record, int recordLength);
int  getRecordLength(char *file_name);


void main(int argc, char *argv[])
{
 char inFile1_name[FileNameSize];
 char inFile2_name[FileNameSize];
 char outFile_name[FileNameSize];

 getFileNames(inFile1_name,inFile2_name,outFile_name,argc,argv);
 
 if ( !recordCompatible(inFile1_name,inFile2_name) )
  printf("ERROR: Files aren't compatible!\n");
 else
  fileUnion(inFile1_name,inFile2_name,outFile_name);

}/* main */


void getFileNames(char *inFile1,char *inFile2,char *outFile,int argc,char *argv[])
{					/* Gets the file names from the command line	*/
 if ( argc != 3 ){
  printf("USAGE: union <inFile1> <inFile2> <outFile> \n");
  exit(1);
 }

 strcpy(inFile1,argv[1]);
 strcpy(inFile2,argv[2]);
 strcpy(outFile,argv[3]);
  
}/* getFileNames */


bool recordCompatible(char *inFile1_name,char *inFile2_name)
{					/* Checks the two descriptor files to see if	*/
 FILE *descFile1;			/*  each field type and length is the same for	*/
 FILE *descFile2;			/*  the records of the two files.		*/
 char  descFile1_name[FileNameSize]; 
 char  descFile2_name[FileNameSize];
 int   fieldNum1=0,fieldNum2=0;
 struct fieldInfo{
  int  field_pos;
  char field_name[12];
  char field_type;
  int  field_length;
 }record1_field[MaxNumOfFields],record2_field[MaxNumOfFields];
 
 strcpy(descFile1_name,inFile1_name);	/* Replaces the original filename extensions	*/
 strcpy(strchr(descFile1_name,'.'),".dsc");				/* with '.dsc'	*/
 strcpy(descFile2_name,inFile2_name);
 strcpy(strchr(descFile2_name,'.'),".dsc");

 if ((descFile1=fopen(descFile1_name,"rb"))==NULL){
  printf("ERROR: %s has no descriptor file! \n",inFile1_name);
  exit(1);
 }
 if ((descFile2=fopen(descFile2_name,"rb"))==NULL){
  printf("ERROR: %s has no descriptor file! \n",inFile2_name);
  exit(1);
 }

 while(!feof(descFile1))
  fread(&record1_field[fieldNum1++],sizeof(struct fieldInfo),1,descFile1);
  
 while(!feof(descFile2))
  fread(&record2_field[fieldNum2++],sizeof(struct fieldInfo),1,descFile2);

 close(descFile1);
 close(descFile2);
 
 if (fieldNum1!=fieldNum2) return(FALSE);
 while(fieldNum1>=0){
  if (record1_field[fieldNum1].field_type!=record2_field[fieldNum1].field_type ||
      record1_field[fieldNum1].field_length!=record2_field[fieldNum1].field_length)
   return(FALSE);
 }
 return(TRUE);
 
}/* recordCompatible */


int  getRecordLength(char *file_name)
{					/* Adds up the field lengths to get the overall	*/
 int   recordLength=0;			/*  record length for the files.		*/
 FILE *descFile;
 char  descFile_name[FileNameSize]; 
 struct fieldInfo{
  int  field_pos;
  char field_name[12];
  char field_type;
  int  field_length;
 }field;
 
 strcpy(descFile_name,file_name);
 strcpy(strchr(descFile_name,'.'),".dsc");
 
 if ((descFile=fopen(descFile_name,"rb"))==NULL){
  printf("ERROR: %s has no descriptor file! \n",file_name);
  exit(1);
 }

 while(!feof(descFile)){
  fread(&field,sizeof(struct fieldInfo),1,descFile);
  recordLength+=field.field_length;
 }
 
 close(descFile);

 return(recordLength);
 
}/* getRecordLength */


bool exist(FILE *file, char *record, int recordLength)
{					/* Checks if 'record' exists in file		*/
 char *testRecord=(char *)malloc(recordLength);
 
 rewind(file);
 while(!feof(file)){
  fread(testRecord,recordLength,1,file);
  if (!memcmp(testRecord,record,recordLength))
   return(TRUE);
 }
 return(FALSE);
}/* exist */


void fileUnion(char *inFile1_name,char *inFile2_name,char *outFile_name)
{					/* Outputs the union of inFile1 and inFile2 to	*/
 FILE *inFile1;				/*  outFile, without duplicates.		*/
 FILE *inFile2;
 FILE *outFile;
 int   recordLength=getRecordLength(inFile1_name);
 char *record=(char *)malloc(recordLength);
 
 
 if ((inFile1=fopen(inFile1_name,"rb"))==NULL){
  printf("ERROR: Couldn't open file : %s \n",inFile1_name);
  exit(1);
 }
 if ((inFile2=fopen(inFile2_name,"rb"))==NULL){
  printf("ERROR: Couldn't open file : %s \n",inFile2_name);
  exit(1);
 }
 if ((outFile=fopen(outFile_name,"wb"))==NULL){
  printf("ERROR: Couldn't open file : %s \n",outFile_name);
  exit(1);
 }

 while(!feof(inFile1)){			/* Copies everything in inFile1 to outFile	*/
  fread(record,recordLength,1,inFile1);
  fwrite(record,recordLength,1,outFile);
 }
 while(!feof(inFile2)){			/* Copies everything in inFile2 that isn't	*/
  fread(record,recordLength,1,inFile2);			/*  already there to outFile.	*/
  if (!exist(outFile,record,recordLength))
   fwrite(record,recordLength,1,outFile);
 }

 close(inFile1);
 close(inFile2);
 close(outFile);
 
}/* union */
