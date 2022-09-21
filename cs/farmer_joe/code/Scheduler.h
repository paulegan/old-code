
// Scheduler.h
// Last update:		10.5.1997
//
// The Scheduler is a list of work objects. It contains a
// STL singly linked list.  The standard list operations have
// simply been replicated.

#ifndef SchedulerHeader
#define SchedulerHeader

#include <SLList.h>

template <class T>
class Scheduler{

public:

	Scheduler()		{};
	~Scheduler()		{};
	clear()			{ if (!list.empty()) list.clear(); };
	append(const T& x)	{ list.append(x); };
	prepend(const T& x)	{ list.prepend(x); };
  T	pop()			{ return(list.remove_front()); };
  int	size()			{ return(list.length()); };
  Bool	empty()			{ return(list.empty()); };

private:

  SLList<T>	list;

};// Scheduler


#endif
