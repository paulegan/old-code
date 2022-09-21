
// slaveTest.cc

#include <simpleSort.h>
#include <ProcessorElement.h>

int main()
{
 ProcessorElement<simpleSort>	pe;
 simpleSort			data;

 while(1){
   pe>>data;
   data.process();
   pe<<data;
 }

 return(0);
}// main
