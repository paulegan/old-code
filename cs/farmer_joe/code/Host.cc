
// Host.cc
// Last update:		12.5.1997

#include <Host.h>


// Host constructor. Calls MPL to add "hostName" and
// sets id. A negative ID returned by the MPL call 
// specifies that the host is already in the VM.
// Sets "rpp" also.
Host::Host(String hostName, RPP rpp)
{
 myID=addHost(hostName);
 if (myID<0)
   if (myID==MPLAddHostFail)
     myID=NoID;
   else{
     myID=-myID;
     dontRemove=TRUE;
   }
 myRPP=rpp;
 myName=hostName;
}// Host




// Host Destructor. Calls MPL to remove host but not
// if it failed to add it in the first place or if
// it was previously in the VM.
Host::~Host()
{
 if (( myID!=NoID) && !dontRemove )
   removeHost(myID);
}// ~Host



// Return relative processing power value.
RPP Host::rpp()
{
 return(myRPP);
}// rpp



// Returns id value.
HostID Host::id()
{
 return(myID);
}// id




// Returns the number of tasks executing on this host with MPL call.
int Host::slaveCount()
{
 return(numTasksOnHost(myID));	
}// slaveCount



// Returns the name of this host.
String Host::name()
{
 return(myName);
}// name



// Marks this host suitable for use by specified SlaveGroup. Can
// return "HostSuccess" or "HostFailure". See note in header file.
HostStatus Host::markSuitable(SlaveGroupID id)
{
 removeFromList(-id);
 return(HostSuccess);
}// markSuitable



// Marks this host unsuitable for use by specified SlaveGroup. Can
// return "HostSuccess" or "HostFailure". See note in header file.
HostStatus Host::markUnsuitable(SlaveGroupID id)
{
 if (!containedInList(-id))
   slaveGroupUsage.prepend(-id);
 return(HostSuccess);
}// markUnsuitable



// Returns whether this host is suitable for the specified SlaveGroup.
Bool Host::suitable(SlaveGroupID id)
{
 return(!containedInList(-id) &&  !containedInList(id));
}// suitable



// Marks this host as used by the specified SlaveGroup. Can
// return "HostSuccess" or "HostFailure". See note in header file.
HostStatus Host::markUsed(SlaveGroupID id)
{
 if (!containedInList(id))
   slaveGroupUsage.prepend(id);
 return(HostSuccess);
}// markUsed



// Marks this host as unused by the specified SlaveGroup. Can
// return "HostSuccess" or "HostFailure". See note in header file.
HostStatus Host::markUnused(SlaveGroupID id)
{
 removeFromList(id);
 return(HostSuccess);
}// markUnused



// Returns whether this host is used by the specified SlaveGroup.
Bool Host::used(SlaveGroupID id)
{
 return(containedInList(id));
}// used



// Removes the specified id from the "slaveGroupUsage" list.
void Host::removeFromList(int id)
{
 for(Pix i=slaveGroupUsage.first(); i; slaveGroupUsage.next(i))
   if (slaveGroupUsage(i)==id){
     slaveGroupUsage.del(i);
     break;
   }
}// removeFromList



// Returns whether the specified id is contained in the
// "slaveGroupUsage" list.
Bool Host::containedInList(int id)
{
 for(Pix i=slaveGroupUsage.first(); i; slaveGroupUsage.next(i))
   if (slaveGroupUsage(i)==id)
     return(TRUE);
 return(FALSE);
}// containedInList
