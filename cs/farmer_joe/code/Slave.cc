
// Slave.cc
// Last update:		10.5.1997

#include <Slave.h>


// Slave constructor. Calls MPL to spawn "slaveName" on
// host, "hostName". If spawn fails suitability is set to 
// "Unsuitable" and id to "NoID".
template <class T> 
Slave<T>::Slave(String slaveName, String hostName)
{
 mySuitability = InitialSuitability;
 myWork = NoWork;
 busyFlag = FALSE;
 hostImOn = hostName2ID(hostName);
 if ((myID=spawnSlave(slaveName,hostName))==MPLSpawnSlaveFail){
   myID=NoID;
   mySuitability = Unsuitable;
 }
}// Slave



// Slave destructor. Calls MPL to remove slave from
// system but not if the initial spawn failed.
template <class T> 
Slave<T>::~Slave()
{
 if (myID!=NoID) removeSlave(myID);
 delete myWork;
}// ~Slave



// Returns current suitability.
template <class T> 
SuitabilityType Slave<T>::suitability(void)
{
 return(mySuitability);
}// suitability



// Save and send work to slave via MPL. The current suitability
// is also sent.  Returns "SlaveSuccess" or "SlaveFailure".
// See header file for notes on suitability.
template <class T> 
SlaveStatus Slave<T>::allocate(T& work)
{
 if (sendTo(myID,(byte*)&work,sizeof(work))==MPLSendFail)
   return(SlaveFailure);
 if (sendTo(myID,(byte*)&mySuitability,sizeof(mySuitability),SuitabilityTag)==MPLSendFail)
   return(SlaveFailure);
 myWork = new T(work);
 busyFlag = TRUE;
 return(SlaveSuccess);
}// allocate



// Frees saved work and receives new suitability from slave 
// process. If receive fails then suitability is left as is.
// Returns "SlaveSuccess" or "SlaveFailure".
// See header file for notes on suitability.
template <class T> 
SlaveStatus Slave<T>::finish()
{
 SuitabilityType* newSuitability;
 int		  size;

 if (nReceiveFrom(myID,(byte**)&newSuitability,size,SuitabilityTag)!=MPLReceiveFail)
   mySuitability = *newSuitability;
 busyFlag = FALSE;
 delete myWork;
 delete newSuitability;
 return(SlaveSuccess);
}// finish



// Returns a copy of the work currently saved or else "NoWork".
template <class T> 
T* Slave<T>::currentWork()
{
 if (myWork)
   return(new T(*myWork));
 else
   return(NoWork);
}// currentWork

	

// Returns id for this slave.
template <class T> 
SlaveID Slave<T>::id()
{
 return(myID);
}// id


// Returns whether the slave is busy or not.
template <class T> 
Bool Slave<T>::busy()
{
 return(busyFlag);
}// id



// Returns the ID of the host on which the slave resides.
template <class T> 
HostID Slave<T>::whereTFAreYou()
{
 return(hostImOn);
}// whereTFAreYou
