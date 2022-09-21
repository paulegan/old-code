#include <stdio.h>

#define NoOfRecords 31 /*463*/

typedef struct{
 char  code[9];
 char  name[33];
 char  address[73];
 char  phone[13];
 char  joinDate[9];
 int   noOfOutlets;
 float annualCost;
 char  paymentStrategy;
 float paymentToDate;
}customerInfo;

void viewFile();
void initFile();

void main()
{
 int choice;
 
 printf("1:view  2:init ");
 scanf("%d",&choice);
 switch(choice){
  case 1:
   viewFile();
  break;
  case 2:
   initFile();
  break;
 }
}/* main */

void viewFile()
{
 FILE *inFile;
 customerInfo aCustomer;
 int x;
 
 inFile = fopen("dataFile","rb");
 for(x=0;x<NoOfRecords;x++){
  fread(&aCustomer,sizeof(customerInfo),1,inFile);
  printf("%4d%10s%34s%4d\n",x,aCustomer.code,aCustomer.name,aCustomer.paymentToDate);
 }
 fclose(inFile);


}/* viewFile */

void initFile()
{
 FILE *outFile;
 customerInfo aCustomer;
 int x;
 
 strcpy(aCustomer.code,"00000000");
 strcpy(aCustomer.name,"");
 strcpy(aCustomer.address,"");
 strcpy(aCustomer.phone,"");
 strcpy(aCustomer.joinDate,"");
 aCustomer.noOfOutlets=0;
 aCustomer.annualCost=0;
 aCustomer.paymentStrategy=' ';
 aCustomer.paymentToDate=0;
 

 outFile = fopen("dataFile","wb");
 for(x=0;x<NoOfRecords;x++)
  fwrite(&aCustomer,sizeof(customerInfo),1,outFile);
 fclose(outFile);

}/* initFile */
