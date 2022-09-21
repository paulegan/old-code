// Most public methods on this object execute the processMessgaes method as part
// of the call.


// SlaveGroup.cc 
// Last update:	10.5.1997


#include <SlaveGroup.h>

// The SlaveGroup(String FileName, HostNameSet hostlist) constructor restricts
// the slaves to run on the specified hosts. All other constructors only provide
// a hint as to how many hosts to use, and the slavegroup is free to grab as many
// machines as it needs to process all the messages.


// Simplest slavegroup constructor, takes the name of the executable to spawn,
// gets a new id, and grabs as many hosts as it can.
template <class T>
SlaveGroup<T>::SlaveGroup(String FileName){
		myEXE=FileName;
		mySlaveGroupID=theFarm.newSlaveGroupID();
		getHosts(OneHost);
		setInitialStatus();
}

// Slavegroup constructor, takes the name of the executable to spawn and
// the number of hosts to use. Gets a new id, and grabs the specified number of hosts.
template <class T>
SlaveGroup<T>::SlaveGroup(String FileName, unsigned int numberOfHosts ){
		myEXE=FileName;
		mySlaveGroupID=theFarm.newSlaveGroupID();
		getHosts(numberOfHosts);
		setInitialStatus();
}

// Slavegroup constructor, takes the name of the executable to spawn and
// the percentage of hosts to use. Gets a new id, and grabs the specified  
// hosts.
template <class T>
SlaveGroup<T>::SlaveGroup(String FileName, Percentage percentageHosts){
	myEXE=FileName;
	mySlaveGroupID=theFarm.newSlaveGroupID();
	getHosts(int(percentageHosts.ofTotal(theFarm.noOfHosts())));
	setInitialStatus();
}



// Slavegroup constructor, takes the name of the executable to spawn and
// a set of hosts to use. Gets a new id, and grabs the specified hosts.
template <class T>
SlaveGroup<T>::SlaveGroup(String FileName, HostNameSet hostlist){
		myEXE=FileName;
		mySlaveGroupID=theFarm.newSlaveGroupID();
		getHosts(hostlist);
		setInitialStatus();
		isRestricted = TRUE;
}

// SlaveGroup destructor. Flushes all messages and kills all slaves.
template <class T>
SlaveGroup<T>::~SlaveGroup(){
	flush(ALL);
	slaveSet.clear();
}


// Adds a messages to the set of messages to be sent. 
template <class T>
void SlaveGroup<T>::send(const T& work){
	scheduler.append(work);
	processMessages();
}
	

// Blocking receive of a message. While there is not a message to return, it 
// calls processMessages.
template <class T> 
T SlaveGroup<T>::receive(void){
	T temp;
	while ( resultSet.empty() ) processMessages();
	temp = resultSet.pop();
	return temp;
}
	

// Clears the contents of the sets. Parameters may be ALL, RESULTS or JOBS, to
// clear the associated sets.
template <class T>	
void SlaveGroup<T>::flush(Contents c ){
	if ( c == ALL ){
		scheduler.clear();
		resultSet.clear();
	}
	else if ( c == RESULTS )
		resultSet.clear();	
	else if ( c == JOBS )
		scheduler.clear();
}
	

// Clears all sets, unrestricts the processor, and grabs as many hosts as possible.
template <class T> 
void SlaveGroup<T>::reboot(void){
	flush(ALL);
	slaveSet.clear();
	if ( isRestricted ){
		theFarm.unRestrict(mySlaveGroupID);
		isRestricted = FALSE;
	}
	getHosts();
}


// Specifies a set of hosts to move the processor to.
template <class T>
void SlaveGroup<T>::moveTo(HostNameSet h){
	isMoving = isRestricted  = TRUE;
	if ( !h.empty() )
		theFarm.restrictHostsTo(h,mySlaveGroupID);
}



// Reduces the slaveSet to just the currently busy slaves, killing all other.
template <class T>
void SlaveGroup<T>::liposuction(void){
	Slave<T>* idleSlave;
	HostID host;
	
	while ( idleSlave = getNextIdleSlave()){
		host = idleSlave->whereTFAreYou();
		theFarm.markHostUsed(host,mySlaveGroupID);
		slaveSet.remove(idleSlave->id());
	}	
}


// Allows the slavegroup to grab as many hosts as it needs to process messages.
// Would be used after previously restrcting the slavegroup to a certain host set.
template <class T> 
void SlaveGroup<T>::unRestrict(void){
	theFarm.unRestrict(mySlaveGroupID);
	isRestricted = FALSE;
	processMessages();
}


// Allows the processor to change the percentage of hosts it is using.
template <class T>
void SlaveGroup<T>::changeHostGroupUtilisationTo(Percentage p){
	int change;
	if ((change = int(p.numberToChange(slaveSet.size(),theFarm.noOfHosts(),p))) > 0 )
		getHosts(change);
	else
		while ( change++ )
			rmSlowestHost();
	processMessages();
}

// Increases the percentage of hosts in use, default increase is 10 percent.
template <class T>
void SlaveGroup<T>::increaseHostGroupUtilisationBy(Percentage p){
	if ( p.value() == 100 ){
		theFarm.unRestrict(mySlaveGroupID);
		getHosts();
	}
	else getHosts(p.numberToChange(slaveSet.size(),theFarm.noOfHosts(),p));
	processMessages();
}

// Decreases the percentage of hosts in use, default decrease is 10 percent.
template <class T>
void SlaveGroup<T>::decreaseHostGroupUtilisationBy(Percentage p){
	int numberOfHostsToKill 
		= p.numberToChange(slaveSet.size(),theFarm.noOfHosts(),p);
		 
	while ( !((--p) == Percentage(0)) ) rmSlowestHost();
	processMessages();
}


// Returns a rating of the slavegroup efficiency based on the average suitability of the
// slaves. This should only be used in comparsion with a rating of another slavegroup
// of the same type.
template <class T>
unsigned int SlaveGroup<T>::rating(void){
	
	unsigned int average=0;
		
	for(Pix i=slaveSet.first(); i; slaveSet.next(i))
		average+=slaveSet(i)->suitability();
	
	if ( slaveSet.size() )
		return average/slaveSet.size();
	else
		return WorstPossibleSuitability; //worst possible suitability..
}

// Returns true if there are any messages to process or if any slave is busy, else false.
template <class T>
Bool SlaveGroup<T>::busy(void){
	Bool busySlave = FALSE;
	processMessages();
		
	for(Pix i=slaveSet.first(); i; slaveSet.next(i)){
		if (slaveSet(i)->busy())
			busySlave = TRUE;
	}
	return ( scheduler.size() || busySlave );
}


// Returns number of hosts in use by the slavegroup.
template <class T>
unsigned int SlaveGroup<T>::numberOfHostsInUse(void){
	processMessages();
	return slaveSet.size();
}


// Returns the number of hosts that could possibly be used, ie. the cardinality of the
// farm.
template <class T>
unsigned int SlaveGroup<T>::numberOfPossibleHosts(void){
	processMessages();
	return theFarm.noOfHosts();
}


// Returns the percentageOfHostsInUse by the slavegroup.
template <class T>
Percentage SlaveGroup<T>::percentageOfHostsInUse(void){
	processMessages();
	return Percentage(slaveSet.size(),theFarm.noOfHosts());
}


// Returns a hostnameset of the current hosts in use by the slavegroup.
template <class T>
HostNameSet SlaveGroup<T>::currentHosts(void){
	processMessages();
	return theFarm.myHosts(mySlaveGroupID); 
}


// Returns the cardinality of the scheduler set of messages.
template <class T>
unsigned int SlaveGroup<T>::jobsRemaining(){
	processMessages();
	return scheduler.size();
}

// Returns the cardinality of the incoming set of messages.
template <class T>
unsigned int SlaveGroup<T>::answersReady(){
	processMessages();
	return resultSet.size();
}


/**********************************************************************************
 **********************************************************************************
 			PRIVATE METHODS..........
 **********************************************************************************
 *********************************************************************************/


// Sends as many messages as possible
template <class T>	     	
void SlaveGroup<T>::sendMessages(void){
	Slave<T>* s;
	while( !scheduler.empty() && (s = getNextIdleSlave()) ){
		T temp = scheduler.pop();
		s->allocate(temp);
	}
	if ( !scheduler.empty()  )
		getHosts();
}

// Removes the slowest slave in use by the slaveGroup. If that slave is busy then its
// work is readded to the scheduler.

template <class T>	     	
void SlaveGroup<T>::rmSlowestHost(){
	Slave<T>* worstSlave = slaveSet(slaveSet.first());
	
	for(Pix i=slaveSet.first(); i; slaveSet.next(i))
		if ( slaveSet(i)->suitability() > worstSlave->suitability() )
			worstSlave = slaveSet(i);
	
	
	if(worstSlave->busy()){
		//T* temp = ();
		scheduler.prepend(*(worstSlave->currentWork()));
	}
	theFarm.markHostUnused(worstSlave->whereTFAreYou(),mySlaveGroupID);
	slaveSet.remove(worstSlave->id());
}

// 
// Used to remove a dead slave from the slaveGroup. It readds the messages that the slave
// was working on to the scheduler, marks the host it was running on as unused, and
// removes the slave from the slaveSet.
template <class T>	     	
void SlaveGroup<T>::burySlave(SlaveID sid){
	Slave<T>* s = slaveSet(sid);
	if ( s ){
		HostID h = s->whereTFAreYou();
		scheduler.append(*(s->currentWork()));
		theFarm.markHostUnused(h, mySlaveGroupID);
		slaveSet.remove(sid);
	}
}


// Attempts to move the slaveGroup to a new set of hosts. If the slaveSet is empty
// then the move is carried out and the computations continue, else it removes aall idle
// slaves. ie. The move is carried out only after all messages are received back from the
// slaves, while waiting for these no further messages are sent out.
template <class T>	     	
void SlaveGroup<T>::move(void){
	Slave<T>* s;
	if ( slaveSet.empty() ){
		isMoving = FALSE; 
		return;
	}
	else while ( s = getNextIdleSlave() ){
			theFarm.markHostUnused(s->whereTFAreYou(),mySlaveGroupID);
			slaveSet.remove(s->id());
	     }
}



// Receives a message from a slave, adding it to the resultSet, and marks the slave
// as free.
template <class T> 
Bool SlaveGroup<T>::receiveFromSlave(SlaveID id){
	T* incoming = new T;
	int size = 42;//this could be anything, or nothing in fact.
	if (nReceiveFrom(id , (byte**)&incoming, size)==MPLReceiveFail){
		if (incoming) delete incoming;
		return FALSE;
	};
	resultSet.append(*incoming);
	slaveSet(id)->finish();
	delete incoming;
	return TRUE;
}


// This method carries out the following:
// Checks for slave exits, burying and dead slaves.
// Receives any messages that are ready.
// If the slaveGroup is moving then it tries to move 
// else sends as many messages as possible.
template <class T> 
void SlaveGroup<T>::processMessages(void){

	//Check for slave exits..	
	SlaveID sid;
	while(sid = checkForSlaveExit())
	  burySlave(sid);
		
	//receive all messages
	for(Pix i=slaveSet.first(); i; slaveSet.next(i))
		if (slaveSet(i)->busy())
			receiveFromSlave(slaveSet(i)->id());
	
	if ( isMoving )
		move();	
	else				
		sendMessages();
}	

// Marks the slavegroup as not restricted and not moving.
template <class T> 
void SlaveGroup<T>::setInitialStatus(void){
  	isRestricted = 
  	isMoving = FALSE;
}  	
  	
// Attempts to add the numberOfHosts specified as a parameter to the slaveGroup
template <class T> 
void SlaveGroup<T>::getHosts(int numberOfHosts){
	Host* host;
	
	if ( numberOfHosts == AllHosts ) 
		numberOfHosts = theFarm.noOfHosts();
	
	while(numberOfHosts--){
		host=theFarm.next(mySlaveGroupID);
		if ( host ) 
			spawnSlave(host);
	}
	
}        

// Attempts to add the set of hosts specified as a parameter to the slaveGroup
template <class T> 
void SlaveGroup<T>::getHosts(HostNameSet hostlist){
	Host* h;
	while ( !hostlist.empty() )
	if (h=theFarm.addHost(hostlist.remove_front()))
			spawnSlave(h);
}


// Attempts to spawn a slave  passed as a parameter to the slaveSet
template <class T> 
void SlaveGroup<T>::spawnSlave(Host* h){	     							
	Slave<T>* slave = new Slave<T>(myEXE,h->name());	
	if ( slave ){
		if (slave->id()==NoID)
			delete slave;
		else 	{
			slaveSet.prepend(slave);
			theFarm.markHostUsed(h->id(),mySlaveGroupID);
		}
	}
}


// Picks the best slave from the idle slave subset of the slaveSet.
template <class T>
Slave<T>* SlaveGroup<T>::getNextIdleSlave(void){
	if ( slaveSet.empty() ) return NULL;
	
	Slave<T>* bestSlave = NULL;	
	
	for(Pix i = slaveSet.first(); i; slaveSet.next(i))
		if ( !slaveSet(i)->busy() && 
		    ( !bestSlave ||
		    (slaveSet(i)->suitability() < bestSlave->suitability()) ))
			bestSlave = slaveSet(i);
	return bestSlave;
}

