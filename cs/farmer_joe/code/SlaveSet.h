
// SlaveSet.h
// Last update:		10.5.1997
//
// The SlaveSet is a list of pointers to slave objects. It contains a
// STL doubly linked list.  The standard list operations have been
// extended to allow for lookups on slave id.  On removal of a list
// node the slave object is destructed.

#ifndef SlaveSetHeader
#define SlaveSetHeader

#include <Slave.h>
#include <DLList.h>


template<class T> 
class  SlaveSet{

public:

		SlaveSet()		{};
		~SlaveSet()		{ clear(); };
		clear()			{ if (!list.empty()) {for(Pix i=list.first(); i; list.next(i)) delete list(i);  list.clear();} };
		prepend(Slave<T>* s)	{ list.prepend(s); };
  int		size()			{ return(list.length()); };
		remove(SlaveID id)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->id()==id) {delete list(i); list.del(i); break;} };
  Slave<T>*	operator()(SlaveID id)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->id()==id) return(list(i)); return(NULL); };
  Bool		contains(SlaveID id)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->id()==id) return(TRUE); return(FALSE); };
  Pix		first()			{ return(list.first()); };
  void		next(Pix& i)		{ list.next(i); };
  Slave<T>*	operator()(Pix i)	{ return(list(i)); };
  Bool		empty()			{ return(list.empty()); };

private:

  DLList<Slave<T>*>	list;

};//class SlaveSet

#endif
