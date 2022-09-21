#include <iostream.h>
#include <Processor.h>
#include <myClass.h>
#include <twoIntClass.h>

int main(){
 intClass ints,answer;
 myClass incInts;
 twoIntClass twoInts;
 
 Processor<myClass> p("slaveMaster","inter2");
 Processor<intClass> p2("numberInc");
 Processor<twoIntCLass>* p3;
 
 for(int i=0,ints.number=0;i<100;i++,ints.number++)
 	p2 << ints;
 
 p3 = new Processor<twoIntCLass>("twoIntInc");
 
 for(int i=0;i<100;i++,incInts.item2=answer.number){
 	p2 >> answer;
 	twoInts.intOne = answer.number;
 	twoInts.intTwo = incInts.item2;
 	*p3 << twoInts;
 }
 
 
 	
 
 cout << "Now exiting." << endl;
 return EXIT_SUCCESS;
}

