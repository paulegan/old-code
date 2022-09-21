
// processor.cc		Manual instantiation file for PFM
// Last Update:		12.5.1997

#include <iostream.h>

#include <Farm.h>
#include <Farm.cc>
Farm theFarm;

#include <Host.h>
#include <Host.cc>

#include <MPL.h>
#include <MPL.cc>

#include <ProcessorElement.h>
#include <ProcessorElement.cc>

#include <Processor.h>
#include <Processor.cc>

#include <SlaveGroup.h>
#include <SlaveGroup.cc>

#include <Slave.h>
#include <Slave.cc>

#include <Percentage.h>
#include <Percentage.cc>

#include "../Demo/simpleSort.h"
template class ProcessorElement<simpleSort>;
template class Processor<simpleSort>;
template class SlaveGroup<simpleSort>;
template class Slave<simpleSort>;

#include "../Debug/myClass.h"
template class ProcessorElement<myClass>;
template class Processor<myClass>;
template class SlaveGroup<myClass>;
template class Slave<myClass>;

#include "../Demo/niall/intClass.h"
template class ProcessorElement<intClass>;
template class Processor<intClass>;
template class SlaveGroup<intClass>;
template class Slave<intClass>;

#include "../Demo/niall/Triple1/twoIntClass.h"
template class ProcessorElement<twoIntClass>;
template class Processor<twoIntClass>;
template class SlaveGroup<twoIntClass>;
template class Slave<twoIntClass>;
