
#include <ProcessorElement.h>


// Constructor. Enrolls process in VM and sets id.
template <class T>
ProcessorElement<T>::ProcessorElement()
{
 joinVirtualMachine();
 SlaveID id=myID();
}// ProcessorElement



// Destructor. Removes process from VM.
template <class T>
ProcessorElement<T>::~ProcessorElement()
{
 leaveVirtualMachine();
}// ~ProcessorElement



// Receives message from parent process. Also receives
// current suitability and sets a start time for the
// calculation on the new suitability.
template <class T>
T* ProcessorElement<T>::receive()
{
 T*	message;
 int	size;

 receiveFromParent((byte**)&message,size);
 receiveFromParent((byte**)&currentSuitability,size,SuitabilityTag);
 startTime=time(NULL);
 return(message);
}// receive



// Calculates new suitability and returns this to parent
// along with the result message.
template <class T>
ProcessorElement<T>::send(const T& message)
{
 time_t			deltaT = time(NULL)-startTime;
 SuitabilityType	newSuitability = (SuitabilityType)
			(( SuitabilityVariance*(float)(*currentSuitability) ) +
			( (1.0-SuitabilityVariance)*(float)deltaT ));

 sendToParent((byte*)&newSuitability,sizeof(newSuitability),SuitabilityTag);
 sendToParent((byte*)&message,sizeof(message));
}// send



// A nicer way of receiving data.
template <class T>
ProcessorElement<T>::operator>>(T& t)
{
 t = *receive();
}// operator>>



// A nicer way to send data.
template <class T>
ProcessorElement<T>::operator<<(T& t)
{
 send(t);
}// operator>>
