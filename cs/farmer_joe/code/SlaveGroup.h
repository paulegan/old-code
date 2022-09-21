// SlaveGRoup.h		Header file for SlaveGroup.cc
// Last update:		10.5.1997	
// 
// For each processor object created there is an associated slavegroup.
// The slavegroup is responsible for collecting incoming messgaes, allocating
// them to free machines, and holding results until the user is ready to collect
// them.
//
// The 3 most important data structures in this object are the scheduler,
// resultSet and slaveSet.
//
// Scheduler: holds outgoing messages and decides what order the 
//	      messages are sent.
//
// ResultSet: holds all messages destined for the programmer.
//
// SlaveSet: holds all available slaves.
//
// The most important method of this object is the processMessages method.
// This method is called every time the slavegroup is called. It checks for,
// and recovers from, slave failure, receives incoming messages, and sends as
// many messages as possible. If it cannot not find a free slave to send a 
// message to, it tries to get more hosts from the farm to spawn slaves on.


#ifndef SlaveGroupHeader 
#define SlaveGroupHeader

#include <String.h>
#include <Farm.h>
#include <Bool.h>
#include <Slave.h>
#include <Host.h>
#include <Percentage.h>
#include <ResultSet.h>
#include <SlaveSet.h>
#include <Scheduler.h>
#include <HostNameSet.h>
#include <limits.h>

#define AllHosts -1
#define OneHost 1
#define WorstPossibleSuitability INT_MAX

typedef enum{ALL,RESULTS,JOBS} Contents;

template <class T>
class SlaveGroup{
  public:
	SlaveGroup(String);
	SlaveGroup(String , unsigned int );
	SlaveGroup(String , Percentage);
	SlaveGroup(String , HostNameSet);	   
	~SlaveGroup();
	
	void flush(Contents);

	void send(const T&);
	T receive(void);
	
	void unRestrict(void);
	void reboot();
	void moveTo(HostNameSet);
	void liposuction(void);
	
	void changeHostGroupUtilisationTo(Percentage);
	void increaseHostGroupUtilisationBy(Percentage);
	void decreaseHostGroupUtilisationBy(Percentage);
	
	unsigned int rating(void);
	Bool busy(void);
	unsigned int numberOfHostsInUse(void);
	unsigned int numberOfPossibleHosts(void); 
        Percentage percentageOfHostsInUse(void); 
        HostNameSet currentHosts(void);  		
	unsigned int jobsRemaining(void);
	unsigned int answersReady(void);
	
  private:
		
	void sendMessages(void);
	void rmSlowestHost();
	void burySlave(SlaveID);
	void move(void);
	
	Bool receiveFromSlave(SlaveID);
	void processMessages(void);
	void setInitialStatus(void);
	void getHosts(int = OneHost);
	void getHosts(HostNameSet);
	void spawnSlave(Host*);
	
	Slave<T>* getNextIdleSlave(void);
		
	Bool isMoving;
	Bool isRestricted;
	SlaveGroupID mySlaveGroupID;
	String myEXE;
  	ResultSet<T> resultSet;
	SlaveSet<T>  slaveSet;
	Scheduler<T> scheduler;

};//class SlaveGroup

#endif
	
	
