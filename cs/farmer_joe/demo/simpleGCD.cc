// simpleGCD.cc
// simple test

#include <iostream.h>
#include <Processor.h>
#include <simpleGCD.h>

int main(){
 const int	numOfWorkObjects = 5;
 simpleGCD	data[numOfWorkObjects];
 int		count = numOfWorkObjects;

 cout << "Starting test.." << endl;
 Processor<simpleGCD> p("simpleGCDPE");

 cout << "Sending..." << endl;
 while(--count){
   p << data[count];
   cout << count << ',' << p.jobsRemaining() << ',' << p.answersReady() <<endl;
 }

 cout << "Receiving..." << endl;
 count = numOfWorkObjects;
 while(--count){
   p >> data[count];
   data[count].display();
 }

 cout << "End of test.." << endl;
 return(EXIT_SUCCESS);
}
