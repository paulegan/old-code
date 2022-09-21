#include <iostream.h>
#include <std.h>
#include <triple.h>
#include <twoIntClass.h>

int main(){
	Triple<twoIntClass> t;
	twoIntClass i,r;
	int f;
	
	t.setInput(&i); 
	t.setOutput(&r);
	
	do{
	 cout << endl << "Enter 2 integers: " << endl;
	 cin >> i.numberOne >> i.numberTwo;
	 cout << "Enter the function you want to perform, from:" << endl
	      << "add(1), multiply(2), subtract(3), divide(4) or end(0)" << endl;
	 cin >> f;   
	
	 if ( f ){
		t.setFunction(f);
	        cout << r.result << " is the result." << endl;
	 }	
	}while ( f );
	
	return EXIT_SUCCESS;
}
