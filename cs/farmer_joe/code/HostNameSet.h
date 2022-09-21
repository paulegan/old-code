
// HostNameSet.h
// Last update:		10.5.1997
//
// The HostNameSet is simply a list of strings.  It inherits
// the STL singly linked list.  The clear method is extended
// so that clear isn't called is the list is empty.

#ifndef HostNameSetHeader
#define HostNameSetHeader

#include <SLList.h>

class HostNameSet : public SLList<String>
{

public:

 void	clear()	{ if (!empty()) SLList<String>::clear(); };

};// HostNameSet

#endif
