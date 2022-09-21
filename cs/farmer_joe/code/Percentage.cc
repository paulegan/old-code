#include <Percentage.h>
#include <iostream.h>


// Percentage constructor.  Takes in an integer and checks whether it
// is a valid percentage. ie. between 0 and 100.  If not, sets it to
// a valid val.
Percentage::Percentage(int x)
{
 if (x > 100)
   val = 100;
 else if(x < 0)
   val = 0;
 else
   val = x;
}// Percentage



// Another percentage constructor
Percentage::Percentage() : val(0)
{
}



// Percentage constructor. Takes in two integers which represent a
// fraction and returns a percentage representation of this fraction.
// Checks for division by zero and if this is the case, returns 42.
// The first arguement is the numerator, the second the denominator.
Percentage::Percentage(int x, int y)
{
 if (y==0)
   val = 42;
 else
   val = (int)(x*100)/y;
 if (val > 100)
   val = 100;
 else if (val < 0)
   val = 0;
}// Percentage



// Percentage descructor.
Percentage::~Percentage()
{
}



// Takes in an integer called total, gets a percentage of total
// and returns this integer.  The actual percentage which is caluculated
// is the val of the callin object.
unsigned int Percentage::ofTotal(int total)
{
 return( (total/100)*val );
}// ofTotal




// Adds the val of b to current object (a) and returns a new percentage
// which has the  val of the sum. If the result is not a 
// valid Percentage (ie. > 100), then sets sum to 100
Percentage Percentage::operator+(Percentage& b)
{
 val+=b.val;
 if(val  > 100)
    val = 100;
 if(val < 0)
    val = 0;
 return(*this);
}// operator+




int Percentage::value()
{
 return(val);
}// value



// Subtracts the val of Percentage b from current object (a) and returns 
// a new percentage which has the val a-b. If the result is
// negative, sets val to zero.
Percentage Percentage::operator-(Percentage& b)
{
 val-=b.val;
 if(val > 100)
    val = 100;
 if(val < 0)
    val = 0;
 return(*this);
}// operator-



Percentage Percentage::operator--()
{
 val--;
 return *this;
}// operator-



// Equality operator. Returns true if val of object is equal to b.val,
// false otherwise.
Bool Percentage::operator==(Percentage& b)
{
 return(val == b.val);
}// operator==



// Greater than operator. Returns true if val of object is greater than
// b.val, otherwise false. 
Bool Percentage::operator>(Percentage& b)
{
 return(val > b.val);          
}//    operator>           
               

// Less than operator.  Returns true if val of object is greater than 
// b.val, otherwise false.
Bool Percentage::operator<(Percentage& b)
{
 return(val < b.val);
}// operator<



// Assignment operator. Assigns the val of Percentage b to
// Percentage a and returns modified a.  Percentage a (and obviously
// Percentage b) must already exist.
Percentage Percentage::operator=(Percentage& a)
{
 val = a.val;
 return *this;
}// operator=


// Takes three arguements. The first is the number presently in use,
// the second is the total number available and the third is the 
// percentage of the total that the user wants to be in use.
// Returns the number which must be added to or subtracted from
// the number in use to bring it to the amount the user requires to be
// in use
int Percentage::numberToChange(int numInUse,int numAvailable,Percentage wanted)
{
 return((numAvailable*wanted.val)/100) - numInUse;
}// numberToChange
