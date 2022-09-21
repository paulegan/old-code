// simpleSort.cc
// simple test

#include <iostream.h>
#include <Processor.h>
#include <simpleSort.h>

int main(){
 const int	numOfWorkObjects = 10;
 int		count = numOfWorkObjects;
 simpleSort	data;

 cout << "Starting test.." << endl;
 Processor<simpleSort> p("simpleSortPE");

 cout << "Sending..." << endl;
 while(--count){
   p << data;
   cout << count << ',' << p.jobsRemaining() << ',' << p.answersReady() <<endl;
 }

 cout << "Receiving..." << endl;
 count = numOfWorkObjects;
 while(--count){
   p >> data;
   if (!data.correctlyProcessed())
     cout << "Error" << endl;
 }

 cout << "End of test.." << endl;
 return(EXIT_SUCCESS);
}
