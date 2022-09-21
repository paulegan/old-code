
// slaveTest.cc

#include <simpleGCD.h>
#include <ProcessorElement.h>

int main()
{
 ProcessorElement<simpleGCD>	pe;
 simpleGCD			data;

 while(1){
   pe>>data;
   data.process();
   pe<<data;
 }

 return(0);
}// main
