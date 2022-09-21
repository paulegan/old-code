#ifndef ProcessorHeader
#define ProcessorHeader

#include <SlaveGroup.h>
#include <config.h>

template<class T> 
class Processor{
	public:
	Processor(String);
	Processor(String , unsigned int);
	Processor(String , Percentage);
	Processor(String , HostNameSet);
	
	~Processor();
	
	void operator<<(const T&);
 	void operator>>(T&);
	
	void useAllHosts(void);
	void reboot(void);
	
	Bool busy();
	void flush(Contents c = ALL); //Possible values are ALL RESULTS JOBS
	void moveTo(HostNameSet);
	void removeIdleHosts(void);
	
	void changeHostGroupUtilisationTo(Percentage P);
	void increaseHostGroupUtilisationBy(Percentage P = DefaultLevel);
	void decreaseHostGroupUtilisationBy(Percentage p = DefaultLevel);

	HostNameSet currentHosts();
	unsigned int numberOfHostsInUse(void);
	unsigned int numberOfPossibleHosts(void);
	Percentage hostUtilisation();
	unsigned int performance();
	unsigned int  jobsRemaining();
	unsigned int  answersReady();
	
private:
	SlaveGroup<T> slaveGroup;
}; //Processor Object
#endif
