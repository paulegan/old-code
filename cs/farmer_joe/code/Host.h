
// Host.h		Header file for Host.cc
// Last update:		10.5.1997
//
// There is a direct mapping between the existance of a host object
// and a host in the virtual machine.  On instantiation of a host,
// a MPL call is made to add the host to the host.  If the host is
// already in the VM then the host object will represent that host.
// On destruction the host is removed from the VM only if it was
// added by the construction of that host object.
//
// The data stored in this class are the hosts name, the hosts id
// ( given by the MPL ) and a list of SlaveGroup ID's.
//
// This list contains information about how each hosts is being used
// by the SlaveGroups.  A positive entry in this list indicates that
// there is a slave of that type spawned on the host - used. A
// negative entry indicates that the host is not to be used by that
// slave type - unsuitable. If "SlaveGroupID" is not contained in
// the list then it is not used and suitable.

#ifndef HostHeader
#define HostHeader


#include <MPL.h>
#include <DLList.h>
#include <Bool.h>


#define HostSuccess	0
#define HostFailure	-1
#define NoID		-1

typedef int RPP;
typedef int SlaveGroupID;
typedef int HostStatus;	



class Host {

public:

	        Host(String, RPP);
	        ~Host();
  HostID        id();
  RPP	        rpp();
  HostStatus    markSuitable(SlaveGroupID);
  HostStatus    markUnsuitable(SlaveGroupID);
  Bool	        suitable(SlaveGroupID);
  HostStatus    markUsed(SlaveGroupID);
  HostStatus    markUnused(SlaveGroupID);
  Bool	        used(SlaveGroupID);
  int	        slaveCount();
  String        name();

private:

  void		removeFromList(int);
  Bool		containedInList(int);

  String        myName;
  HostID        myID;
  RPP	        myRPP;
  DLList<int>	slaveGroupUsage;
  Bool		dontRemove;

};//class Host


#endif
