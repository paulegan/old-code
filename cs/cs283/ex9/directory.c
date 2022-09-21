
/*				CS283  Assignment 9				*/
/*			  Paul Egan (94312974) 8 March 96			*/

/* directory.c - A program that provides the basic functions to manage a 	*/
/*	telephone directory using a linked list.				*/

#include <stdio.h>
#include "tester.h"

#define NameStringLen	20
#define DoesntExist	-1	/* Returned by lookUp if name doesn't exist	*/


typedef int  phoneNumber;
typedef char subName[NameStringLen];
typedef struct sub_tag{
 phoneNumber 	 phoneNum;
 subName	 name;
 struct sub_tag *next;
}subscriber;

subscriber  *mkEmpty();
phoneNumber  lookUp(subscriber *directory, subName name);
subscriber  *insert(subscriber *directory, subName name, phoneNumber num);
subscriber  *delete(subscriber *directory, phoneNumber num);
void printDirectory(subscriber *directory);


void main()
{
 subscriber *directory;
 char 	    *aName="A Test Name";
 int	     aNumber=123456;
 
 directory=mkEmpty();
 test(directory==NULL);
 test(lookUp(directory,aName)==DoesntExist);
 directory=delete(directory,aNumber);
 directory=insert(directory,aName,aNumber); 
 directory=insert(directory,"Another Name!",123457);
 test(lookUp(directory,aName)==aNumber);
 directory=delete(directory,aNumber);
 test(lookUp(directory,aName)==DoesntExist);

}/* main */


subscriber *mkEmpty()
{
 return(NULL);
}/* mkEmpty */

subscriber  *insert(subscriber *directory, subName name, phoneNumber num)
{
 subscriber *newSubscriber=(subscriber *)malloc(sizeof(subscriber));
 subscriber *sub=directory; 
 subscriber *prevSub=NULL;

 newSubscriber->phoneNum = num;
 strcpy(newSubscriber->name,name);

 while(sub!=NULL && sub->phoneNum>num){
  prevSub = sub;
  sub = sub->next;
 }
  
 newSubscriber->next = sub;
 if (sub==directory)
  directory=newSubscriber;
 else
  prevSub->next=newSubscriber;
 
 return(directory);
}/* insert */

subscriber  *delete(subscriber *directory, phoneNumber num)
{
 subscriber *sub=directory; 
 subscriber *prevSub=NULL;
  
 while(sub!=NULL && sub->phoneNum!=num){
  prevSub = sub;
  sub = sub->next;
 }
 
 if (sub!=NULL){
  if (sub==directory)
   directory=directory->next;
  else
   prevSub->next=sub->next;
  free(sub);
 }
 
 return(directory);
}/* delete */


phoneNumber  lookUp(subscriber *directory, subName name)
{
 subscriber *sub=directory;
 
 while(sub!=NULL && strcmp(sub->name,name))
  sub = sub->next;
 
 if (sub==NULL)
  return(DoesntExist);
 else
  return(sub->phoneNum);
}/* lookUp */

void printDirectory(subscriber *directory)
{
 printf("Entries:\n");
 while(directory){
  printf("%s %d\n",directory->name,directory->phoneNum);
  directory=directory->next;
 }
}/* printDirectory */

