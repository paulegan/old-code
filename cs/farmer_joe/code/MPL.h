
// MPL.h		Header file for MPL.cc
// Last update:		10.5.1997
//
// These are a set of function calls that server as an interface
// to the Message Passing Library - in this case, PVM.  All the
// standard MPL calls are supported.
//
// Whenever a process is spawned or a host added a pvm_notify call
// is made so that if the process or host fails a message as to that
// effect will be sent.
//
// Many of the PVM calls require a list of host names ( char** ). Since
// these functions only take one host name at a time, as a C++ String,
// many of these functions copy the contents of the String to a char*
// and then pass the address of this to the PVM calls as if it was a
// list with one element.


#ifndef MPLHeader
#define MPLHeader

#include <String.h>
#include <pvm3.h>
#include <std.h>
#include <config.h>

// Return values:
#define MPLSuccess		0
#define MPLFailure		-1
#define MPLAddHostFail		-3
#define MPLSpawnSlaveFail	-4
#define MPLSendFail		-5
#define MPLReceiveFail		-6
#define MPLRemoveFail		-7

// Message tags:
#define MPLHostFail		1
#define MPLSlaveExit		2
#define MPLMessage		3


// Data types:
typedef int  HostID;
typedef int  SlaveID;
typedef int  MPLStatus;
typedef int  MessageTag;
typedef char byte;



HostID		addHost(String);
SlaveID		spawnSlave(String, String);
SlaveID		myID();
SlaveID		parentID();
HostID		checkForHostFail();
SlaveID		checkForSlaveExit();
MPLStatus	sendTo(SlaveID, byte*, int, MessageTag = MPLMessage);
MPLStatus	receiveFrom(SlaveID, byte**, int&, MessageTag = MPLMessage);
MPLStatus	nReceiveFrom(SlaveID, byte**, int&, MessageTag = MPLMessage);
MPLStatus	sendToParent(byte*, int, MessageTag = MPLMessage);
MPLStatus	receiveFromParent(byte**, int&, MessageTag = MPLMessage);
MPLStatus	removeSlave(SlaveID);
MPLStatus	removeHost(HostID);
MPLStatus	leaveVirtualMachine();
MPLStatus	joinVirtualMachine();
String		hostID2Name(HostID);
HostID		hostName2ID(String&);
int		numTasksOnHost(HostID);

#endif
