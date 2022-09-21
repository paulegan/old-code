
// ResultSet.h
// Last update:		10.5.1997
//
// The ResultSet is a list of work objects. It contains a
// STL singly linked list.  The standard list operations have
// simply been replicated.

#ifndef ResultSetHeader
#define ResultSetHeader

#include <SLList.h>

template <class T>
class ResultSet{

public:

	ResultSet()		{};
	~ResultSet()		{};
	clear()			{ if (!list.empty()) list.clear(); };
	append(const T& x)	{ list.append(x); };
	prepend(const T& x)	{ list.prepend(x); };
  T	pop()			{ return(list.remove_front()); };
  int	size()			{ return(list.length()); };
  Bool	empty()			{ return(list.empty()); };

private:

  SLList<T>	list;

};// ResultSet


#endif
