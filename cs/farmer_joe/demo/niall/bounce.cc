#include <iostream.h>
#include <Processor.h>
#include <intClass.h>

int main(){
	intClass job,result;
	const String machine("inter");
	String newHost;
	String whereToGo;
	HostNameSet machineToGoTo;
	
	Processor<intClass> p("numberInc",1);
	
	
	for(;;){
		cout << "What machine do you want to compute on?" << endl
		     << "Enter a number between 1 and 4, or 0 to exit." << endl;
		cin >> whereToGo;
		if ( whereToGo=="0" ) break;
		
		cout << "Enter the number you want to increment." << endl;
		cin >> job.number;
		
		newHost = machine+whereToGo; 
		machineToGoTo.clear();
		machineToGoTo.append(newHost);
		
		p.moveTo(machineToGoTo);
		p << job;
		p >> result;
		
		cout << result.number << " is the answer from " << newHost << endl;
	}
	
	return EXIT_SUCCESS;
}
