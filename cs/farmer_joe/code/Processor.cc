#include <Processor.h>
#include <String.h>


template<class T> 
Processor<T>::Processor(String FileName) : slaveGroup(FileName){
}	    
		    	 
	
template <class T>	
Processor<T>::Processor(String FileName, unsigned int NumberOfHosts)
	   :slaveGroup(FileName,NumberOfHosts){
}

template <class T>	
Processor<T>::Processor(String FileName, Percentage HostsAvailable)
	   :slaveGroup(FileName,HostsAvailable){
}

template <class T>
Processor<T>::Processor(String FileName, HostNameSet Hosts)
	   :slaveGroup(FileName,Hosts){
}


template <class T>
Processor<T>::~Processor(){
}	   



template <class T>
void Processor<T>::operator<<(const T& t){
	slaveGroup.send(t);
}

template <class T>
void Processor<T>::operator>>(T& t){
	t = slaveGroup.receive();
}

template <class T>
void Processor<T>::useAllHosts(void){
        slaveGroup.unRestrict();
}


template <class T>
void Processor<T>::reboot(){
	slaveGroup.reboot();
}

template <class T>
Bool Processor<T>::busy(){
        return slaveGroup.busy();
}

template <class T>
void Processor<T>::flush(Contents c ) {	
        slaveGroup.flush(c);
}


template <class T>
void Processor<T>::moveTo(HostNameSet h){
        slaveGroup.moveTo(h);
}

template <class T>
void Processor<T>::removeIdleHosts(void){
        slaveGroup.liposuction();
}
template <class T>
void Processor<T>::changeHostGroupUtilisationTo(Percentage p){
        slaveGroup.changeHostGroupUtilisationTo(p);
}


template <class T>
void Processor<T>::increaseHostGroupUtilisationBy(Percentage p){
        slaveGroup.increaseHostGroupUtilisationBy(p);
}

template <class T>
void Processor<T>::decreaseHostGroupUtilisationBy(Percentage p ){
        slaveGroup.decreaseHostGroupUtilisationBy(p);
}


template <class T>
HostNameSet Processor<T>::currentHosts(){
	return slaveGroup.currentHosts();
}


template <class T>
unsigned int Processor<T>::numberOfHostsInUse(){
	return slaveGroup.numberOfHostsInUse();
}


template <class T>
unsigned int Processor<T>::numberOfPossibleHosts(){
	return slaveGroup.numberOfPossibleHosts();
}


template <class T>
Percentage Processor<T>::hostUtilisation(){ 
        return slaveGroup.percentageOfHostsInUse();
}


template <class T>
unsigned int Processor<T>::performance(){
	return slaveGroup.rating();
}

template <class T>
unsigned int Processor<T>::jobsRemaining(){
	return slaveGroup.jobsRemaining();
}


template <class T>
unsigned int Processor<T>::answersReady(){
	return slaveGroup.answersReady();
}




