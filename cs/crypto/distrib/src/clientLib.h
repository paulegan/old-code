
/*  clientLib.h			lumpy@cs.ucc.ie			14.8.1996	*/

/*  Header file for clientLib.c							*/


#define Success			0
#define Failure			-1
#define	NotRegistered		1

#define DESKeyStrSize		12
#define PrivateKeyStrSize	990		/* Do NOT change these!!	*/
#define PublicKeyStrSize	363


/*  startSession: connects client to 'server' as 'name'.  'server' is of the	*/
/*	form "serverName[@host]".  Will fail if 'server' cannot be found or if	*/
/*	if 'name' is already being used.					*/

void  startSession(const char *name, const char *server);


/*  endSession: disconnects client from server.  Returns 'Success' if no error. */

int   endSession();


/*  sendMessageTo: sends 'message' to client 'principal'.  If client isn't	*/
/*	registered then 'message' is saved by server and sent to the client	*/
/*	upon connecting.  Returns 'Success' if no error.			*/

int   sendMessageTo(const char *principal, const char *message);


/*  receiveMessage:  blocks for message and then returns a pointer to the 	*/
/*	reveived message.							*/

char *receiveMessage();


/*  newMessage:  Checks for a new message.  Returns 'TRUE' or 'FALSE'.		*/

int   newMessage();


/*  interceptFrom:  Requests to intercept all messages sent from 'principal'.	*/
/*	'principal' MUST be registered.  Returns 'NotRegistered' or 'Success'.	*/

int   interceptFrom(const char *principal);


/*  interceptTo:  Requests to intercept all messages sent to 'principal'.	*/
/*	'principal' MUST be registered.  Returns 'NotRegistered' or 'Success'.	*/

int   interceptTo(const char *principal);


/*  listenFrom:  Requests to receive all messages sent from 'principal'.	*/
/*	'principal' MUST be registered.  Returns 'NotRegistered' or 'Success'.	*/

int   listenFrom(const char *principal);


/*  listenTo:  Requests to receive all messages sent to 'principal'.		*/
/*	'principal' MUST be registered.  Returns 'NotRegistered' or 'Success'.	*/

int   listenTo(const char *principal);


/*  newTimeStamp:  Returns the number of seconds since 00:00:00 GMT,  Jan.  1,	*/
/*	1970, as a string.							*/

char *newTimeStamp();


/* validTimeStamp: Returns 'TRUE' if the time stamp 'ts' is within the 'range'	*/
/*	( measured in seconds ).  Otherwise returns 'FALSE'. 			*/

int validTimeStamp(char *ts, long range);


/* newDESKey:  Returns a random string for use as a DES key.			*/

char *newDESKey();

/* DESencrypt:  Returns a string which is 'message' encrypted using DES CFB	*/
/*	under key 'keyStr' and ASCII-encoded according to RFC-1421.		*/

char *DESencrypt(char *message, char *keyStr);


/* DESdecrypt:  Returns a string which is 'message' ASCII-decoded and decrypted	*/
/*	using DES CFB under key 'keyStr'.					*/

char *DESdecrypt(char *message, char *keyStr);


/* newRSAKeys:  Generates a private and public key pair for RSA encryption.	*/
/*	The char arrays passed to this function should be at least		*/
/*	'PublicKeyStrSize' and 'PrivateKeyStrSize' in size respectively.	*/

void newRSAKeys(char *publicKeyStr, char *privateKeyStr);


/* RSAencrypt:  Returns a string which is 'message' encrypted using RSA under	*/
/*	key 'keyStr' and ASCII-encoded according to RFC-1421.			*/

char *RSAencrypt(char *message, char *keyStr);


/* RSAdecrypt:  Returns a string which is 'message' ASCII-decoded and decrypted	*/
/*	using RSA under key 'keyStr'.						*/

char *RSAdecrypt(char *message, char *keyStr);


/* storeKey:  Saves 'key' under 'name'.						*/

void  storeKey(const char *name, const char *key);


/* getKey:  Returns the key stored under 'name'.  Returns 'NULL' if none.	*/

char *getKey(const char *name);


/* removeKey:  Removes named key from stored list.				*/

void  removeKey(const char *name);


/* saveKeys:  Saves stored keys in a file.					*/

void  saveKeys(const char *fileName);


/* loadKeys:  Loads keys from a file.						*/

void  loadKeys(const char *fileName);


/* freeKeyList:  Deletes list of stored keys.					*/

void  freeKeyList();

