
// ProcessorElement.h	Header file for ProcessorElement.cc
// Last update:		11.5.1997
//
// The ProcessorElement class is used to send and receive work and
// result to and from the parent process.  As the name suggests many
// ProcessorElement's make up the processor.  For each Slave object
// in the processor, there should be a corresponding process with
// a ProcessorElement object.  Only one ProcessorElement object should 
// exist in any one process.
//
// The only public methods to send and receive messages, however,
// ProcessorElement objects also do some extra work when these methods
// are called - the calculation of suitability values.
//
// The relative suitability values are used to compare the performance
// of Slaves/ProcessorElements.  When data is received by a
// ProcessorElements the current suitability is also received and the
// current time is recorded.  When the data has been processed and
// being sent to the Processor the new suitability is calculated and
// also sent.  Currently suitability is taken as the average time
// taken to process data.  Therefor the lower the value the more
// suitable the Slave/ProcessorElement.  The function is as follows:
// 		s = (sv*s) + ( (1-sv)*deltaT
// where s is the suitability, sv is the suitability variance
// ( defined is the config file. ) and deltaT is the time taken to
// process the data.

#ifndef ProcessorElementHeader
#define ProcessorElementHeader

#include <MPL.h>
#include <Slave.h>
#include <time.h>
#include <config.h>


template <class T>
class ProcessorElement{

public:

	ProcessorElement();
	~ProcessorElement();
	operator>>(T&);
	operator<<(T&);

private:

  T*			receive();
			send(const T&);
  time_t		startTime;
  SuitabilityType*	currentSuitability;

}; // ProcessorElement

#endif
