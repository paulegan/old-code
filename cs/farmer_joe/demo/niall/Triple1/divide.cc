
// pe.cc

#include <twoIntClass.h>
#include <ProcessorElement.h>

void main()
{
 ProcessorElement<twoIntClass>	pe;
 twoIntClass			data;

 for(;;){
   pe >> data;
   data.divide();
   pe << data;
 }


}// main
