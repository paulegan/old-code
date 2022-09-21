
// Farm.cc
// Last update:		10.5.1997

#include <Farm.h>


// Farm constructor. Adds the process to the VM. Determines which
// file to use as the host file and calls "initializeHostsFrom" if
// this file exists.
Farm::Farm()
{
 joinVirtualMachine();

 String        fileName;
 ifstream      hostFile;

 if ((fileName = getenv(PFMFileEnvVar))==NULL) {
   fileName = getenv(HOMEEnvVar);
   fileName += PFMFileName;
 }

 hostFile.open(fileName);
 if (hostFile) initializeHostsFrom(hostFile);
 hostFile.close();
}// Farm



// Farm destructor.
Farm::~Farm()
{
 hostSet.~HostSet();
 leaveVirtualMachine();
 //what about the rejects list??
}// ~Farm



// Reads the host file and adds referenced hosts. Lines starting with
// a '#' are taken to be comments. Currently accepts input of the
// form "hostname rpp" where rpp is optional.
void Farm::initializeHostsFrom(ifstream& hostsFile)
{
 String 	       line, hostname, rppStr;
 RPP		       rpp;
 char		       ch;

 while ((ch = hostsFile.peek())!=EOF) {
   switch (ch) {
     case Comment :    hostsFile.getline(line,MaxFileLineWidth,'\n');
		       break;
     case '&' :        
     case ' ' :
     case '\t':
     case '\n':        hostsFile >> ch;
		       break;
     default  :        hostsFile.getline(line,MaxFileLineWidth,'\n');
		       hostname = strtok((char*)line,Delimiters);
		       rppStr = strtok(NULL,Delimiters);
		       if (rppStr!=NULL)
			 rpp = atoi((char*)rppStr);
		       else
			 rpp = DefaultRPP;
        	       addHost(hostname,rpp);
   }
 }
}// initializeHosts



// Finds host in hostSet with given ID and calls method on
// that host to mark it as used by the specified SlaveGroup.
// Returns "FarmSuccess" or "FarmFailure".
FarmStatus Farm::markHostUsed(HostID hID, SlaveGroupID sID)
{
  Host *host = hostSet(hID);
  if (host->id()!=NoID && host->markUsed(sID)==HostSuccess)
    return FarmSuccess;
  return FarmFailure;
}// markHostUsed



// Finds host in hostSet with given ID and calls method on
// that host to mark it as unused by the specified SlaveGroup.
// Returns "FarmSuccess" or "FarmFailure".
FarmStatus Farm::markHostUnused(HostID hID, SlaveGroupID sID)
{
 Host *host = hostSet(hID);
 if (host->id()!=NoID && host->markUnused(sID)==HostSuccess)
   return FarmSuccess;
 return FarmFailure;
}// markHostUnused



// Adds a new host to the VM and return pointer to it. Instantiates a
// new host object. If the additiion of the host failed then the name and
// rpp are added to the reject list and NULL is returned.
Host* Farm::addHost(String hostname, RPP rpp)
{
 Host*         newHost;
 rejectHost    reject;

 if (hostSet.contains(hostname))
   return(hostSet(hostname));
 
 newHost = new Host(hostname,rpp);
 if (newHost->id()!=NoID){
   hostSet.add(newHost);
   return(newHost);
 }
 
 reject.name = newHost->name();
 reject.rpp = newHost->rpp();
 rejects.prepend(reject);
 delete newHost;
 return(NULL);
}// addHost



// Returns the most suitable host for given SlaveGroup.  First checks
// for host failure and adds failed hosts to the reject list.  Next
// tries to find the most suitable host.  The most suitable host for
// a SlaveGroup is defined to be: a host that isn't marked
// unsuitable, has the highest relative processing power of those
// suitable and has the least number of slaves than hosts of the same
// RPP.  If a suitable host isn't found then "tryToStartRejects" is
// called. If a suitable host still isn't found then NULL is
// returned, otherwise a pointer to that host.
Host* Farm::next(SlaveGroupID id)
{
 Host	       *mostSuitable = NULL,*currentHost;
 HostID        hID;
 rejectHost    reject;
 
 while(hID=checkForHostFail()){        // Deal with failed hosts.
   currentHost = hostSet(hID);
   reject.name=currentHost->name();
   reject.rpp=currentHost->rpp();
   rejects.prepend(reject);
   hostSet.remove(hID);
 }

redo:
 for(Pix h=hostSet.first(); h; hostSet.next(h))
   if ( hostSet(h)->suitable(id) && 
       ( !mostSuitable || 
         ( hostSet(h)->rpp() > mostSuitable->rpp()) ||
	 ( (hostSet(h)->rpp() == mostSuitable->rpp()) && 
	   (hostSet(h)->slaveCount() < mostSuitable->slaveCount())
	 )
       )
      )
     mostSuitable = hostSet(h);

 if ( !mostSuitable && (tryToStartRejects()==FarmSuccess) )
   goto redo;

 return(mostSuitable);
}// nextInSlaveGroup



// Returns the next unused SlaveGroupID.
SlaveGroupID Farm::newSlaveGroupID()
{
 static int idCounter = FirstSlaveGroupID;
 return(idCounter++);
}// newSlaveGroupID();



// Iterates through the hostSet marking all 
// hosts suitable for this SlaveGroupID.
// Can return "FarmSuccess" or "FarmFailure".
FarmStatus Farm::unRestrict(SlaveGroupID id)
{
 FarmStatus returnStatus = FarmSuccess;
 
 for(Pix h=hostSet.first(); h; hostSet.next(h))
   if ( !hostSet(h)->suitable(id) )
     if (hostSet(h)->markSuitable(id)==HostFailure)
       returnStatus = FarmFailure;

 return(returnStatus);
}// unRestrict
 

// Iterates through the host set marking specified hosts suitable for
// the SlaveGroup and unspecified hosts unsuitable. Also tries to add
// the specified hosts to the VM in case they don't already exist.
// Can return "FarmSuccess" or "FarmFailure".
FarmStatus Farm::restrictHostsTo(HostNameSet hostNames, SlaveGroupID id)
{
 for(Pix n=hostNames.first(); n; hostNames.next(n))
   addHost(hostNames(n));

 for(Pix h=hostSet.first(); h; hostSet.next(h)){
   for(Pix n=hostNames.first(); n && hostNames(n)!=hostSet(h)->name(); hostNames.next(n));
   if (n)
     hostSet(h)->markSuitable(id);
   else
     hostSet(h)->markUnsuitable(id);
 }
 
 return(FarmSuccess);
}// restrictHostsTo



// Returns the number of hosts in the hostSet.
int Farm::noOfHosts()
{
 return(hostSet.length());
}// noOfHosts();



// Returns a list of host names specifying the hosts being used
// by the given SlaveGroup.
HostNameSet Farm::myHosts(SlaveGroupID id)
{
 HostNameSet hostNames;

 for(Pix h=hostSet.first(); h; hostSet.next(h))
   if (hostSet(h)->suitable(id))
      hostNames.prepend(hostSet(h)->name());

 return(hostNames);
}// myHosts



// Move through the list of reject hosts and attempt
// to add them.
// Can return "FarmSuccess" or "FarmFailure".
FarmStatus Farm::tryToStartRejects()
{
 FarmStatus		returnVal=FarmFailure;
 rejectHost		h;
 SLList<rejectHost>	temp(rejects);
 
 if (!rejects.empty()) rejects.clear();
 while (!temp.empty()){
   h = temp.remove_front();
   if (addHost(h.name,h.rpp)!=NULL)
     returnVal = FarmSuccess;
 }
 return(returnVal);
}// tryToStartRejects
