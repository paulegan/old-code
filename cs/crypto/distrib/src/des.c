
/* des.c			lumpy@cs.ucc.ie			22.8.1996	*/

#include <config.h>
#include <desLib.h>

extern Socket *mySocket;

char *newDESKey()
{
 char *newKey = (char*)memalloc(NewDESKeySize);
 
 Sprintf(mySocket,"%d",NewDESKey);
 Sscanf(mySocket,"%s",newKey);
 
 getResult();
 return(newKey);
}/* newDESKey */


char *DESencrypt(char *message, char *keyStr)
{
 long		  length = (long)(strlen(message)+1);
 char		 *temp = (char*)memalloc(length);
 char		 *out = (char*)memalloc((int)(length*1.4));
 des_key_schedule ks; 
 des_cblock 	  key;
 des_cblock 	  ivec = DES_CFB_Ivec;
 int 		  ivec_len = 0;
 int		  outLen;
 
 des_string_to_key(keyStr,&key);
 des_set_odd_parity(&key);
 des_set_key(&key,ks);
 des_cfb64_encrypt(message,temp,length,ks,&ivec,&ivec_len,DES_ENCRYPT);
 
 ASCIIencodeBlock(temp,length,out,&outLen);
 out[outLen] = '\0';
 
 free(temp);
 return(out); 
}/* DESencrypt */


char *DESdecrypt(char *message, char *keyStr)
{
 long		  length = (long)strlen(message);
 char		 *out = (char*)memalloc(length);
 char		 *temp = (char*)memalloc(length);
 des_key_schedule ks; 
 des_cblock 	  key;
 des_cblock 	  ivec = DES_CFB_Ivec;
 int 		  ivec_len = 0;
 int		  tempLen;
  
 ASCIIdecodeBlock(temp,&tempLen,message,length);
 
 des_string_to_key(keyStr,&key);
 des_set_odd_parity(&key);
 des_set_key(&key,ks);
 des_cfb64_encrypt(temp,out,tempLen,ks,&ivec,&ivec_len,DES_DECRYPT);
 
 free(temp);
 return(out);
}/* DESencrypt */
