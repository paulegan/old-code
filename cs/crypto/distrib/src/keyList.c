
/* keyList.c			lumpy@cs.ucc.ie			29.8.1996	*/


#include <keyList.h>

keyListType *keyList = NULL;

void storeKey(const char *name, const char *key)
{
 keyListType *newKey = (keyListType*)memalloc(sizeof(keyListType));
 keyListType *curKey = keyList;
 keyListType *prevKey = NULL;

 newKey->name = (char*)memalloc(strlen(name)+1);
 strcpy(newKey->name,name);
 newKey->key = (char*)memalloc(strlen(key)+1);
 strcpy(newKey->key,key);
 newKey->next = NULL;

 while(curKey){
   prevKey = curKey;
   curKey = curKey->next;
 }
 
 if (curKey == keyList)
   keyList = newKey;
 else
   prevKey->next = newKey;

}/* storeKey */


char *getKey(const char *name)
{
 keyListType *curKey = keyList;

 while(curKey && strcmp(curKey->name,name))
   curKey = curKey->next;

 if (curKey)
   return(curKey->key);
 else
   return(NULL);
}/* getKey */


void removeKey(const char *name)
{
 keyListType *curKey = keyList;
 keyListType *prevKey = NULL;

 while(curKey && strcmp(curKey->name,name)){
   prevKey = curKey;
   curKey = curKey->next;
 }
 
 if (curKey){
   prevKey->next = curKey->next;
   free(curKey->name);
   free(curKey->key);
   free(curKey);
 }

}/* removeKey */


void saveKeyList(const char *fileName)
{
 FILE 		*file;
 keyListType 	*curKey = keyList;
 
 if (!(file = fopen(fileName,"w"))){
   printf("Couldn't open %s! \n",fileName);
   exit(-1);
 }
 
 while(curKey){
   fprintf(file,"%s %s ",curKey->name,curKey->key);
   curKey = curKey->next;
 }
 
 fclose(file);
 
}/* saveKeyList */


void loadKeyList(const char *fileName)
{
 FILE *file;
 char  name[100];
 char  key[2000];
  
 if (!(file = fopen(fileName,"r"))){
   printf("Couldn't open %s! \n",fileName);
   exit(-1);
 }
 
 freeKeyList();
 
 while(!feof(file)){
   fscanf(file,"%s %s",name,key);
   if (!feof(file))
     storeKey(name,key);
 }
 fclose(file);

}/* loadKeyList */


void freeKeyList()
{
 keyListType *curKey = keyList;
 keyListType *prevKey = NULL;

 while(curKey){
   prevKey = curKey;
   curKey = curKey->next;
   free(prevKey->name);
   free(prevKey->key);
   free(prevKey);
 }
 free(keyList);
 keyList = NULL;
 
}/* freeKeyList */


void printKeyList()
{
 keyListType *curKey = keyList;

 while(curKey){
   printf("%s:\t \"%s\" \n",curKey->name,curKey->key);
   curKey = curKey->next;
 }

}/* printKeyList */

