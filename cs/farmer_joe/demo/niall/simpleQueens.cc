#include <iostream.h>
#include <Processor.h>
#include <nqueenSolution.h>

#define defaultN 8
#define nameOfSolver "nqueenSolver"

int main(int argc, char** argv){
 String N = defaultN;
 
 if ( argc == 1 ){
 	cout << "Using default N of 8." << endl;
 else
 	N = 1[argv];
 	
 			
 Processor<nqueenSolution> p(nameOfSolver+" "+N);
 
 for (int i=0,max=p.noOfPossibleHosts(); i<10*max;i++)
 	p << **createGuess;

 p >> solution;
 
 cout << solution;
 
 cout << "Now exiting." << endl;
 return EXIT_SUCCESS;
}

