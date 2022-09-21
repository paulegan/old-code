
// slaveTest.cc

#include <USER_CLASS.h>
#include <ProcessorElement.h>

int main()
{
 ProcessorElement<USER_CLASS>	pe;
 USER_CLASS			data;

 while(1){
   pe>>data;
   data.USER_PROCESS_METHOD();
   pe<<data;
 }

 return(0);
}// main
