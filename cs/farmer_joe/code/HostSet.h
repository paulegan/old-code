
// HostSet.h
// Last update:		10.5.1997
//
// The HostSet is a list of pointers to host objects. It contains a
// STL doubly linked list.  The standard list operations have been
// extended to allow for lookups on host name and id.  On removal
// of a list node the host object is destructed.

#ifndef HostSetHeader
#define HostSetHeader

#include <Host.h>
#include <DLList.h>

class HostSet{

public:

		HostSet()		{};
		~HostSet()		{ for(Pix i=list.first(); i; list.next(i)) delete list(i); };
		add(Host* h)		{ list.prepend(h); };
  int		length()		{ return(list.length()); };
		remove(HostID id)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->id()==id) {delete list(i); list.del(i); break;} };
  Host*		operator()(HostID id)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->id()==id) return(list(i)); return(NULL); };
  Host*		operator()(String name) { for(Pix i=list.first(); i; list.next(i)) if (list(i)->name()==name) return(list(i)); return(NULL); };
  Bool		contains(String name)	{ for(Pix i=list.first(); i; list.next(i)) if (list(i)->name()==name) return(TRUE); return(FALSE); };
  Pix		first()			{ return(list.first()); };
  void		next(Pix& i)		{ list.next(i); };
  Host*		operator()(Pix i)	{ return(list(i)); };

private:

  DLList<Host*>	list;

};//class HostSet


#endif
