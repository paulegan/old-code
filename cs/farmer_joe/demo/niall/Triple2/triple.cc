#include <triple.h>

template <class T>
void Triple<T>::execute(void){
	p = new Processor<T>(function);
	p << *input;
	p >> *output;
	delete p;
	p = NULL;
}

