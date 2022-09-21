#include <Integer.h>
#include <RndInt.h>
#include <ACG.h>
#include <std.h>
#include <limits.h>
#include <time.h>

#ifndef simpleSortHeader
#define simpleSortHeader

#define NumElements 100

class simpleSort{

public:

	simpleSort()		{};
	~simpleSort()		{};
  void	process();
  bool	correctlyProcessed();
  void	display();

private:

  unsigned contents[NumElements];

};


#endif
