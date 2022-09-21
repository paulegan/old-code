
#ifndef PercentageHeader
#define PercentageHeader

#include <Bool.h> 
#include <iostream.h>

class Percentage{

public:
		Percentage(int);
		Percentage();
		Percentage(int,int);
		~Percentage();
  int		value(); 
  Percentage	operator+(Percentage&);
  Percentage 	operator-( Percentage&);
  Percentage	operator--(void);
  Bool		operator==(Percentage&);
  Bool		operator>(Percentage&);
  Bool		operator<(Percentage&);
  int		numberToChange(int,int,Percentage);
  Percentage  	operator=(Percentage&); 
  unsigned int 	ofTotal(int);

private:

  int		val;

}; // end Percentage


#endif
