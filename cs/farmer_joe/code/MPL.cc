
// MPL.cc
// Last update:		10.5.1997

#include <MPL.h>


// Enroll process in PVM.  If pvmd cannot be found exits with
// an error.
MPLStatus joinVirtualMachine()
{
 if (myID()<0) exit(EXIT_FAILURE);
}// joinVirtualMachine



// Remove process from PVM.
MPLStatus leaveVirtualMachine()
{
 sleep(1);			// Why? - Emm, 'cause it works! 8-)
 pvm_exit();
}// leaveVirtualMachine



// If a host has failed return its ID else return 0.
// Checks for host failure by checking for messages
// with tag set by pvm_notify when host was added.
HostID checkForHostFail()
{
 HostID hostID;
 if (!pvm_nrecv(-1, MPLHostFail))
   return(0);
 pvm_upkint(&hostID,1,1);
 return(hostID);
}// checkForHostFail



// If a slave has exited return its ID else return 0.
// Checks for slave exit by checking for messages
// with tag set by pvm_notify when slave was spawned.
SlaveID checkForSlaveExit()
{
 SlaveID slaveID;
 if (!pvm_nrecv(-1, MPLSlaveExit))
   return(0);
 pvm_upkint(&slaveID,1,1);
 return(slaveID);
}// checkForSlaveExit



// Adds a host to PVM and returns its ID. First checks to see if the
// host already exists in the VM. If so sets return value to the
// negative of the id. If PVM fails to add the host then
// "MPLAddHostFail" is returned.  Also calls pvm_notify to send a
// message  with tag "MPLHostFail" if the host fails.
HostID addHost(String hostName)
{
 HostID hostID;

 if ((hostID=hostName2ID(hostName))!=MPLFailure){
   pvm_notify(PvmHostDelete,MPLHostFail,1,&hostID);
   return(-hostID);
 }

 char* hostStr = new char[hostName.length()+1];
 strcpy(hostStr,(const char*)hostName);
 pvm_addhosts(&hostStr,1,&hostID);
 delete [] hostStr;

 if (hostID<0)
   return(MPLAddHostFail);
 else
   pvm_notify(PvmHostDelete,MPLHostFail,1,&hostID);
   return(hostID);
}// addHost



// Spawns slave with exec name "slaveName" on host "hostName" and
// returns ID else "MPLSpawnSlaveFail" if encounters a problem.
// pvm_notify called to send message with tag "MPLSlaveExit" when
// slave exits.
SlaveID spawnSlave(String slaveName, String hostName)
{
 SlaveID slaveID;
 char*   hostStr = new char[hostName.length()];
 char*   slaveStr = new char[slaveName.length()];
 int     ret;

 strcpy(hostStr,(const char*)hostName);
 strcpy(slaveStr,(const char*)slaveName);
 ret=pvm_spawn(slaveStr,NULL,PvmTaskHost,hostStr,1,&slaveID);
 delete [] hostStr;
 delete [] slaveStr;

 if (ret!=1) return(MPLSpawnSlaveFail);
 pvm_notify(PvmTaskExit,MPLSlaveExit,1,&slaveID);

 return(slaveID);
}// spawnSlave



// Send message contained in memory block to slave with id.
// Returns "MPLSuccess" if successful otherwise "MPLSendFail".
// "blockSize" is packed so size is known on reception.
MPLStatus sendTo(SlaveID id, byte* blockPtr, int blockSize, MessageTag tag)
{
 pvm_initsend(DataEncodingMethod);
 pvm_pkint(&blockSize,1,1);
 pvm_pkbyte(blockPtr,blockSize,1);
 pvm_send(id,tag);

 return(MPLSuccess);
}// sendTo



// Does a blocking receive for messages from process with given id.
// Returns "MPLSuccess" if successful otherwise "MPLReceiveFail".
MPLStatus receiveFrom(SlaveID id, byte** blockPtr, int& blockSize, MessageTag tag)
{
 pvm_recv(id,tag);
 pvm_upkint(&blockSize,1,1);
 *blockPtr = new byte[blockSize];
 pvm_upkbyte(*blockPtr,blockSize,1);

 return(MPLSuccess);
}// receiveFrom



// Does a non-blocking receive for messages from slave with given id.
// ( Well a timed receive anyway! ) Returns "MPLSuccess" if
// successful otherwise "MPLReceiveFail".
MPLStatus nReceiveFrom(SlaveID id, byte** blockPtr, int& blockSize, MessageTag tag)
{
 int bufid;
 static struct timeval tmout = nReceiveTimeout;
 if ((bufid=pvm_trecv(id,tag,&tmout))<=0)
   return(MPLReceiveFail);
 pvm_upkint(&blockSize,1,1);
 *blockPtr = new byte[blockSize];
 pvm_upkbyte(*blockPtr,blockSize,1);

 return(MPLSuccess);
}// nReceiveFrom



// Kills slave with given id. Returns "MPLSuccess" or "MPLRemoveFail".
MPLStatus removeSlave(SlaveID id)
{
 if (pvm_kill(id))
   return(MPLRemoveFail);
 else
   return(MPLSuccess);
}// removeSlave



// Removes host with given id from PVM. Must first convert id to name.
// Returns "MPLSuccess" or "MPLRemoveFail".
MPLStatus removeHost(HostID id)
{
 int     err;
 String  hostName = hostID2Name(id);
 char*   hostStr = new char[hostName.length()+1];

 strcpy(hostStr,(const char*)hostName);
 pvm_delhosts(&hostStr,1,&err);
 delete [] hostStr;
 if (err<1)
   return(MPLRemoveFail);
 else
   return(MPLSuccess);
}// removeHost




// Send message to parent.
// Returns "MPLSuccess" if successful otherwise "MPLSendFail".
MPLStatus sendToParent(byte* blockPtr, int blockSize, MessageTag tag)
{
 return(sendTo(pvm_parent(),blockPtr,blockSize,tag));
}// sendToParent



// Does a blocking receive for messages from parent.
// Returns "MPLSuccess" if successful otherwise "MPLReceiveFail".
MPLStatus receiveFromParent(byte** blockPtr, int& blockSize, MessageTag tag)
{
 return(receiveFrom(pvm_parent(),blockPtr,blockSize,tag));
}// receiveFromParent



// Converts a host ID to the corresponding host name. Requires
// a search through an array of all the hosts in the VM.
// Returns empty string if not found.
String hostID2Name(HostID id)
{
 String* hostName;
 struct  pvmhostinfo* hostInfo=NULL;
 int     numOfHosts;
 int     numOfArchs;
 int     h=0;

 pvm_config(&numOfHosts,&numOfArchs,&hostInfo);
 while((h<numOfHosts) && (hostInfo[h].hi_tid!=id))
   h++;
 if (h>=numOfHosts)
   hostName = new String("");
 else
   hostName = new String(hostInfo[h].hi_name);
 return(*hostName);
}// hostID2Name



// Converts a host name to the corresponding host id. Requires
// a search through an array of all the hosts in the VM.
// Returns "MPLFailure" if host not found.
HostID hostName2ID(String& hostName)
{
 struct  pvmhostinfo* hostInfo=NULL;
 int     numOfHosts;
 int     numOfArchs;
 int     h=0;

 pvm_config(&numOfHosts,&numOfArchs,&hostInfo);
 while((h<numOfHosts) && (strcmp(hostInfo[h].hi_name,hostName)))
   h++;
 if (h>=numOfHosts)
   return(MPLFailure);
 else
   return(hostInfo[h].hi_tid);
}// hostID2Name




// Returns the PVM tid of this process.
SlaveID myID()
{
 return(pvm_mytid());
}// myID



// Returns the PVM tid of the parent of this process.
SlaveID parentID()
{
 return(pvm_parent());
}// parentID


//
int numTasksOnHost(HostID id)
{
 struct	pvmtaskinfo* taskInfo=NULL;
 int	numTasks;
 
 if (pvm_tasks(id,&numTasks,&taskInfo)<0)
   return(MPLFailure);
 return(numTasks);
}// numTasksOnHost
