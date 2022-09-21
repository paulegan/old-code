
// Farm.h		Header file for Farm.cc
// Last update:		10.5.1997
//
// There should only be one object instantiated from this Farm class.
// This farm object manages all hosts being used by all SlaveGroups.
// On instantiation the PFM host file is read and all hosts 
// referenced are added to the virtual machine.  Hosts can also be
// added on request by a SlaveGroup.  The farm is exclusively used
// by SlaveGroup objects.  The main method used by these is the
// "next" method.  This returns the most suitable host for a SlaveGroup.
// The "mark" and "restrict" calls control how the "next" method
// operates.
//
// The two data structures contained in the farm are the hostSet and
// a list of reject hosts.  The hostSet is a list of all hosts that are
// currently in the virtual machine.  The reject list holds info on any
// hosts that failed to be added and are to be tried again.



#ifndef FarmHeader
#define FarmHeader

#include <iostream.h>
#include <fstream.h>
#include <String.h>
#include <std.h>
#include <Host.h>
#include <HostSet.h>
#include <HostNameSet.h>
#include <MPL.h>
#include <config.h>


// Return values:
#define FarmSuccess	0
#define FarmFailure	-1


typedef int		FarmStatus;
typedef struct{
  String name;
  RPP	rpp;
}rejectHost;


class Farm{

public:

		Farm();
		~Farm();
  FarmStatus	markHostUsed(HostID, SlaveGroupID);
  FarmStatus	markHostUnused(HostID, SlaveGroupID);
  Host*		addHost(String,RPP=DefaultRPP);
  Host*		next(SlaveGroupID);
  SlaveGroupID	newSlaveGroupID();
  FarmStatus  	unRestrict(SlaveGroupID);
  FarmStatus	restrictHostsTo(HostNameSet, SlaveGroupID);
  int		noOfHosts();
  HostNameSet 	myHosts(SlaveGroupID);

private:

  void		initializeHostsFrom(ifstream&);
  FarmStatus	tryToStartRejects();
  
  HostSet 		hostSet;
  SLList<rejectHost>	rejects;

};//class Farm


#endif
