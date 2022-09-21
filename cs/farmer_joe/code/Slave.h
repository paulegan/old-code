
// Slave.h		Header file for Slave.cc
// Last update:		10.5.1997
//
// There is a direct mapping between the existance of a slave object
// and a slave process somewhere in the virtual machine. On
// instantiation of a slave, a MPL call is made to spawn the named
// executable on the specified host. The slave destructor calls the
// MPL to kill the slave process.
//
// The data stored by this class are the slaves id ( given by the MPL ),
// the id of the host on which the process is executing, the current
// work and the relative suitability of the slave.
//
// If the slave process associated with the slave object is actually
// processing data, then that data is saved in "myWork".  The data is
// saved so that, if the slave process fails before finishing, another
// slave can be started in its place and be sent that data.  If the slave
// process isn't currently working then "myWork" is set to "NoWork".
// The "busyFlag" is also set whenever the slave is working.
//
// "mySuitability" is used to store the slaves suitability. The relative
// suitability is used to decide which slave to send work. When a slave
// is allocated work the current suitability is sent to the slave process
// along with the work object.  The slave process sends back its new
// suitability value when it is finished processing. See 
// 'ProcessorElement.h' for more info on suitability calculations.



#ifndef SlaveHeader
#define SlaveHeader

#include <Bool.h>
#include <MPL.h>

// Return values:
#define SlaveSuccess		0
#define SlaveFailure		-1
#define Unsuitable		-1
#define NoID			-1
#define InitialSuitability	0
#define NoWork			NULL
#define SuitabilityTag		42	// This could be anything at all -
					// just not one of the tags in MPL.h!

typedef int	SlaveStatus;
typedef int	SuitabilityType;


template <class T> 
class Slave{

public:

			Slave(String, String);
			~Slave();
  SuitabilityType 	suitability();
  SlaveStatus 		allocate(T&);
  SlaveStatus		finish();	
  T* 			currentWork();
  SlaveID		id();
  Bool			busy();
  HostID		whereTFAreYou();

private:

  SlaveID 		myID;
  SuitabilityType	mySuitability;
  T*			myWork;
  Bool			busyFlag;
  HostID		hostImOn;

};// Slave


#endif

