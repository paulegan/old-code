#include <String.h>
#include <iostream.h>
#include <Processor.h>
#include <myClass.h>
#include <std.h>
#include <MPL.h>

extern Farm theFarm;

int main(){
	String dummy;
	Processor<myClass>* p;
	
	p = new Processor<myClass>("pe");
	cout << " just newed.. " << endl;
	sleep(2);
	
	cout << " now deleting.." << endl;
	delete p;
	sleep(2);
	
	cout << "now deleting farm..." << endl;
	theFarm.~Farm();
	sleep(2);
	
	cout << "leaving PVM" << endl;
	leaveVirtualMachine();
	sleep(2);
	
	cout << "type string and hit return to exit.." << endl;
	cin >> dummy;
	return 0;
}
