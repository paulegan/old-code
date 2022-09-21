#include <intClass.h>
#include <ProcessorElement.h>

int main()
{
 ProcessorElement<intClass>	pe;
 intClass			data;

 for(;;){
   pe>>data;
   data.process();
   pe<<data;
 }

 return(0);
}// main
