#include "Test/tfunction.h"

#define USER_CLASS_1 tfunction
//#define USER_CLASS_2 int

#include <iostream.h>


#include <Farm.h>
#include <Farm.cc>
Farm theFarm;

#include <Host.h>
#include <Host.cc>

#include <HostSet.h>
#include <HostSet.cc>

#include <MPL.h>
#include <MPL.cc>

#include <ProcessorElement.h>
#include <ProcessorElement.cc>
template class ProcessorElement<USER_CLASS_1>;
//template class ProcessorElement<USER_CLASS_2>;

#include <Processor.h>
#include <Processor.cc>
template class Processor<USER_CLASS_1>;
//template class Processor<USER_CLASS_2>;

#include <SlaveGroup.h>
#include <SlaveGroup.cc>
template class SlaveGroup<USER_CLASS_1>;
//template class SlaveGroup<USER_CLASS_2>;

#include <Scheduler.h>
template class Scheduler<USER_CLASS_1>;
//template class Scheduler<USER_CLASS_2>;

#include <ResultSet.h>
template class ResultSet<USER_CLASS_1>;
//template class ResultSet<USER_CLASS_2>;


#include <Slave.h>
#include <Slave.cc>
template class Slave<USER_CLASS_1>;
//template class Slave<USER_CLASS_2>;

#include <SlaveSet.h>
#include <SlaveSet.cc>
template class SlaveSet<USER_CLASS_1>;
//template class SlaveSet<USER_CLASS_2>;

#include <List.h>
#include <List.cc>
template class List<int>;
template class List<Host*>;
template class List<rejectHost>;
template class List<USER_CLASS_1>;
template class List<Slave<USER_CLASS_1> *>;
//template class List<Slave<USER_CLASS_2> *>;

#include <ListNode.h>
#include <ListNode.cc>
template class ListNode<int>;
template class ListNode<Host*>;
template class ListNode<rejectHost>;
template class ListNode<USER_CLASS_1>;
template class ListNode<Slave<USER_CLASS_1> *>;
//template class ListNode<Slave<USER_CLASS_2> *>;

#include <Iterator.h>
#include <Iterator.cc>
template class Iterator<Host*>;
template class Iterator<USER_CLASS_1>;

#include <Percentage.h>
#include <Percentage.cc>

#include <IntSet.h>
#include <IntSet.cc>

