
#ifndef Config_header
#define Config_header


#include <bool.h>
#include <socketLib.h>

#define PrincipalNameSize	20
#define TimeStampSize		12
#define NewDESKeySize		12

#define DES_CFB_Ivec		{0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef}

#define DefaultServerName	"cryptoServer"

enum commands {
	NoCommand,
	EndSession,
	StartSession,
	SendMessage,
	ReceiveMessage,
	NewMessage,
	ListenTo,
	ListenFrom,
	InterceptTo,
	InterceptFrom,
	NewTimeStamp,
	ValidTimeStamp,
	NewDESKey,
	NewRSAKeys
};

#define Success			0
#define Failure			-1
#define	NotRegistered		1
#define AlreadyRegistered	2

#endif
