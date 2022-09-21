
/* rsa.c			lumpy@cs.ucc.ie			21.8.1996		*/

#include <rsaLib.h>
#include <stdlib.h>

#define RSAKeySize		508
#define RSAKeyLen		(int)((RSAKeySize+7)/8)
#define RSABlockInSize		(RSAKeyLen-11)
#define RSABlockOutSize		(RSAKeyLen)

#define PrivateKeyStrSize       (size_t)((sizeof(R_RSA_PRIVATE_KEY)*1.4)+2)
#define PublicKeyStrSize        ((int)(sizeof(R_RSA_PUBLIC_KEY)*1.4)+2)

#define PrivateKeyStrID		'R'
#define PublicKeyStrID		'U'


int isPublicKeyStr(char *publicKeyStr)
{
 return(publicKeyStr[strlen(publicKeyStr)-1] == PublicKeyStrID);
}/* isPublicKeyStr */


int isPrivateKeyStr(char *privateKeyStr)
{
 return(privateKeyStr[strlen(privateKeyStr)-1] == PrivateKeyStrID);
}/* isPrivateKeyStr */


char *publicKeyToStr(R_RSA_PUBLIC_KEY *publicKey)
{
 char *keyStr = (char*)memalloc((size_t)PublicKeyStrSize);
 char  temp[PublicKeyStrSize];
 int   keyStrLen;
 
 ASCIIencodeBlock((char*)publicKey,sizeof(R_RSA_PUBLIC_KEY),keyStr,&keyStrLen);
  
 keyStr[keyStrLen] = PublicKeyStrID;
 keyStr[keyStrLen+1] = '\0';
  
 return(keyStr);
}/* publicKeyToStr */


R_RSA_PUBLIC_KEY *strToPublicKey(char *publicKeyStr)
{
 R_RSA_PUBLIC_KEY *publicKey = (R_RSA_PUBLIC_KEY*)memalloc(sizeof(R_RSA_PUBLIC_KEY));
 int size;
 
 ASCIIdecodeBlock((char*)publicKey,&size,publicKeyStr,strlen(publicKeyStr)-1);
 
 return(publicKey);
}/* strToPublicKey */


char *privateKeyToStr(R_RSA_PRIVATE_KEY *privateKey)
{
 char *keyStr = (char*)memalloc(PrivateKeyStrSize);
 char  temp[PrivateKeyStrSize];
 int   keyStrLen;
  
 ASCIIencodeBlock((char*)privateKey,sizeof(R_RSA_PRIVATE_KEY),keyStr,&keyStrLen);
  
 keyStr[keyStrLen] = PrivateKeyStrID;
 keyStr[keyStrLen+1] = '\0';
  
 return(keyStr);
}/* privateKeyToStr */


R_RSA_PRIVATE_KEY *strToPrivateKey(char *privateKeyStr)
{
 R_RSA_PRIVATE_KEY *privateKey = (R_RSA_PRIVATE_KEY*)memalloc(sizeof(R_RSA_PRIVATE_KEY));
 int size;
 
 ASCIIdecodeBlock((char*)privateKey,&size,privateKeyStr,strlen(privateKeyStr)-1);
 
 return(privateKey);
}/* strToPrivateKey */


void newRSAKeys(char *publicKeyStr, char *privateKeyStr)
{
 R_RSA_PUBLIC_KEY  publicKey;
 R_RSA_PRIVATE_KEY privateKey;
 R_RSA_PROTO_KEY   protoKey;
 R_RANDOM_STRUCT   randomStruct;
 
 protoKey.bits = RSAKeySize;
 protoKey.useFermat4 = TRUE;
 
 R_RandomCreate(&randomStruct); 
 R_GeneratePEMKeys(&publicKey,&privateKey,&protoKey,&randomStruct);
 
 strcpy(publicKeyStr,publicKeyToStr(&publicKey));
 strcpy(privateKeyStr,privateKeyToStr(&privateKey));
 
}/* newRSAKeys */


char *RSAencrypt(char *message, char *keyStr)
{
 R_RSA_PUBLIC_KEY  *publicKey;
 R_RSA_PRIVATE_KEY *privateKey;
 R_RANDOM_STRUCT    randomStruct;
 int		    messageLen = strlen(message)+1;
 int		    blockInLen = RSABlockInSize;
 char		   *blockIn = message;
 int		    encryptedStrLen = (((int)(messageLen/blockInLen)+1)*RSAKeyLen);
 char		   *encryptedStr = (char*)memalloc(encryptedStrLen);
 int		    blockOutLen;
 char		   *blockOut = encryptedStr;
 int		    encodedStrLen;
 char		   *encodedStr;
 
 if (isPublicKeyStr(keyStr)){
   R_RandomCreate(&randomStruct);
   
   publicKey = strToPublicKey(keyStr);
   while(strlen(blockIn)>blockInLen){
     RSAPublicEncrypt(blockOut,&blockOutLen,blockIn,blockInLen,publicKey,&randomStruct);
     blockIn += blockInLen;
     blockOut += blockOutLen;
   }
   RSAPublicEncrypt(blockOut,&blockOutLen,blockIn,strlen(blockIn)+1,publicKey,&randomStruct);
 }
 else if (isPrivateKeyStr(keyStr)){
   privateKey = strToPrivateKey(keyStr);
   while(strlen(blockIn)>blockInLen){
     RSAPrivateEncrypt(blockOut,&blockOutLen,blockIn,blockInLen,privateKey);
     blockIn += blockInLen;
     blockOut += blockOutLen;
   }
   RSAPrivateEncrypt(blockOut,&blockOutLen,blockIn,strlen(blockIn)+1,privateKey);
 }
 else
   error(1,"In function \"RSAencrypt\" keyStr was not a RSA key! \n");
 
 encodedStr = (char*)memalloc((int)(((blockOut+blockOutLen)-encryptedStr)*1.4));  
 ASCIIencodeBlock(encryptedStr,encryptedStrLen,encodedStr,&encodedStrLen);
 encodedStr[encodedStrLen] = '\0'; 
 
 return(encodedStr);   
}/* RSAencrypt */


char *RSAdecrypt(char *message, char *keyStr)
{
 R_RSA_PUBLIC_KEY  *publicKey;
 R_RSA_PRIVATE_KEY *privateKey;
 int		    messageLen = strlen(message);
 int		    decryptedStrLen = messageLen;
 char		   *decryptedStr = (char*)memalloc((size_t)decryptedStrLen);
 int		    decodedStrLen = messageLen;
 char		   *decodedStr = (char*)memalloc(decodedStrLen);
 int		    blockInLen = RSABlockOutSize;
 char		   *blockIn = decodedStr;
 int		    blockOutLen;
 char		   *blockOut = decryptedStr;
 
 ASCIIdecodeBlock(decodedStr,&decodedStrLen,message,messageLen); 
  
 if (isPublicKeyStr(keyStr)){
   publicKey = strToPublicKey(keyStr);
   while((decodedStrLen-(blockIn-decodedStr))>blockInLen){
     RSAPublicDecrypt(blockOut,&blockOutLen,blockIn,blockInLen,publicKey);
     blockIn += blockInLen;
     blockOut += blockOutLen;
   }
   RSAPublicDecrypt(blockOut,&blockOutLen,blockIn,blockInLen,publicKey);
 }
 else if (isPrivateKeyStr(keyStr)){
   privateKey = strToPrivateKey(keyStr);
   while((decodedStrLen-(blockIn-decodedStr))>blockInLen){
     RSAPrivateDecrypt(blockOut,&blockOutLen,blockIn,blockInLen,privateKey);
     blockIn += blockInLen;
     blockOut += blockOutLen;
   }
   RSAPrivateDecrypt(blockOut,&blockOutLen,blockIn,blockInLen,privateKey);
 }
 else
   error(1,"In function \"RSAencrypt\" keyStr was not a RSA key! \n");
 
 return(decryptedStr);   
}/* RSAencrypt */
