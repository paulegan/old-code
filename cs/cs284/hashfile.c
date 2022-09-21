#include <stdio.h>
#include <unistd.h>
#include "bool.h"

#define FileSize 		31 /* 463 */
#define CodeStringSize 		9
#define NameStringSize 		33
#define AddressStringSize 	73
#define PhoneStringSize		13
#define JoinDateStringSize	9

typedef struct{
 char  code[CodeStringSize];
 char  name[NameStringSize];
 char  address[AddressStringSize];
 char  phone[PhoneStringSize];
 char  joinDate[JoinDateStringSize];
 int   noOfOutlets;
 float annualCost;
 char  paymentStrategy;
 float paymentToDate;
}customerInfo;

void getNewCustomer();
void displayCustomerInfo();
void recordAPayment();
void deleteACustomer();
bool fileFull();
bool isEmpty(int pos);
customerInfo *customerAtPosition(int  pos);
int positionFor(char code[]);
void insert(customerInfo customer);
void getString(char str[],int size);
void getCharacter(char *ch);

FILE *dataFile;

void main()
{
 bool notFinished=TRUE;
 char operation;
 
 dataFile=fopen("dataFile","rb+");
 while(notFinished){
  printf("Enter command: ");
  getCharacter(&operation);
  switch(operation){
   case '1':
    getNewCustomer();
   break;
   case '2':
    displayCustomerInfo();
   break;
   case '3':
    recordAPayment();
   break;
   case '4':
    deleteACustomer();
   break;
   case 'q':
    notFinished=FALSE;
   break;
   default:
    printf("Invalid command!\n");
   break;
  }/* switch */
 }/* while */
 fclose(dataFile);
 
}/* main */

void getNewCustomer()
{
 customerInfo customer;
 
 if (fileFull()){ 
  printf("The file is full!\n");
  return;
 }
 
 printf("Please enter customers ID code : ");
 getString(customer.code,CodeStringSize);
 printf("%s \n",customer.code);
 printf("%d \n",positionFor(customer.code));
 if (!isEmpty(positionFor(customer.code))){
  printf("Customer already exists!\n");
  return;
 }
 printf("Please enter customers name : ");
 getString(customer.name,NameStringSize);
 printf("Please enter customers address : ");
 getString(customer.address,AddressStringSize);
 printf("Please enter customers phone : ");
 getString(customer.phone,PhoneStringSize);
 printf("Please enter customers join date : ");
 getString(customer.joinDate,JoinDateStringSize);
 printf("Please enter customers no of outlets : ");
 scanf("%d",&customer.noOfOutlets);
 fflush(stdin);
 printf("Please enter customers annual cost : ");
 scanf("%f",&customer.annualCost);
 fflush(stdin);
 printf("Please enter customers payment strategy : ");
 getCharacter(&customer.paymentStrategy);
 printf("Please enter customers payment to date : ");
 scanf("%f",&customer.paymentToDate);
 fflush(stdin);
 
 insert(customer);
}/* getNewCustomer */

void displayCustomerInfo()
{
 customerInfo *customer;
 char code[CodeStringSize];
 int pos;
 
 printf("Please enter customers ID code : ");
 scanf("%s",code); 

 pos=positionFor(code);
 if (isEmpty(pos)){
  printf("Customer doesn't exist!\n");
  return;
 }
 customer=customerAtPosition(pos);
 printf("Code             : %s \n",customer->code);
 printf("Name             : %s \n",customer->name);
 printf("Address          : %s \n",customer->address);
 printf("Phone            : %s \n",customer->phone);
 printf("Join date        : %s \n",customer->joinDate);
 printf("No of outlets    : %d \n",customer->noOfOutlets);
 printf("Annual cost      : %f \n",customer->annualCost);
 printf("Payment strategy : %c \n",customer->paymentStrategy);
 printf("Payment to date  : %f \n",customer->paymentToDate);
 
 free(customer);
}/* displayCustomerInfo */

void recordAPayment()
{
 customerInfo *customer;
 char code[CodeStringSize];
 int pos;
 float paymentAmount;
 
 printf("Please enter customers ID code : ");
 scanf("%s",code); 

 pos=positionFor(code);
 if (isEmpty(pos)){
  printf("Customer doesn't exist!\n");
  return;
 }
 customer=customerAtPosition(pos);

 printf("Please enter the payment amount: ");
 scanf("%f",&paymentAmount);
 customer->paymentToDate+=paymentAmount;
 
 insert(*customer);
}/* recordAPayment */

void deleteACustomer()
{
 customerInfo blankCustomer;
 char code[CodeStringSize];
 int pos;
 
 printf("Please enter customers ID code : ");
 scanf("%s",code); 

 pos=positionFor(code);
 if (isEmpty(pos)){
  printf("Customer doesn't exist!\n");
  return;
 }
 strcpy(blankCustomer.code,"00000000");
 fseek(dataFile,pos,SEEK_SET);
 fwrite(&blankCustomer,sizeof(customerInfo),1,dataFile);
 rewind(dataFile); 
 
}/* deleteACustomer */

bool fileFull()
{
 int x;
 
 for(x=0;x<FileSize;x++)
  if(isEmpty(x))
   return(FALSE);
 return(TRUE);
}/* fileFull */

bool isEmpty(int pos)
{ 
 if (strcmp(customerAtPosition(pos)->code,"00000000"))
  return(FALSE);
 else
  return(TRUE);
}/* isEmpty */

customerInfo *customerAtPosition(int  pos)
{
 customerInfo *customer=(customerInfo *)malloc(sizeof(customerInfo));
 
 fseek(dataFile,pos,SEEK_SET);
 fread(customer,sizeof(customerInfo),1,dataFile);
 rewind(dataFile);
 
 return(customer);
}/* customerAtPosition */


int positionFor(char code[])
{
 int key=1,pos;
 int c=0;
 
 while(code[c])
  key*=code[c++]; 
 
 printf("Key for code (%s) = %d\n",code,key);
 pos=key%FileSize;

 while(!isEmpty(pos) && strcmp(customerAtPosition(pos)->code,code))
  pos=(pos+key)%FileSize;

 return(pos);
}/* positionFor */

void insert(customerInfo customer)
{
 int pos=positionFor(customer.code);
 
 fseek(dataFile,pos,SEEK_SET);
 fwrite(&customer,sizeof(customerInfo),1,dataFile);
 rewind(dataFile); 
}/* insert */

void getCharacter(char *ch)
{
 *ch = getchar();
 getchar();
}/* getCharacter */

void getString(char str[],int size)
{
 int x=0;
 
 fgets(str,size,stdin);
 while(str[x]!=10 && str[x]!=0);
 str[x--]='\0';
 fflush(stdin);
}/* getString */
