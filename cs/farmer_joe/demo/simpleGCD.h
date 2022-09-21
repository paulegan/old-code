#include <Integer.h>
#include <RndInt.h>
#include <ACG.h>
#include <std.h>
#include <limits.h>
#include <time.h>

#ifndef simpleGCDHeader
#define simpleGCDHeader

class simpleGCD{

public:

	simpleGCD() : x(0),y(0),z(0)	{};
	~simpleGCD()			{};
  void	process();
  bool	correctlyProcessed()		{ return(z==(int)gcd(x,y)); };
  void  display(){cout << "Ints: " << x << ',' << y << ',' << z << endl; };

private:

  double	x,y,z;

};

inline void simpleGCD::process()
{
 ACG		gen(time(NULL));
 RandomInteger	rnd(1,LONG_MAX,&gen);

 x=rnd.asDouble();
 y=rnd.asDouble();
 z=hypot(x,y);
 display();
 sleep(10);
}

 
#endif
