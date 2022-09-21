#include <triple.h>
#include <std.h>
template <class T>
void Triple<T>::setInput(T* t){
	input = t;
	IfCompleteExecute;
}

template <class T>
void Triple<T>::setOutput(T* t){
	output = t;
	IfCompleteExecute;
}


template <class T>
void Triple<T>::setFunction(int f){
	function = f;
	IfCompleteExecute;
}

template <class T>
void Triple<T>::execute(void){
	
	switch ( function){
	case 1 : p = new Processor<T>("add");break;
	case 2 : p = new Processor<T>("multiply");break;
	case 3 : p = new Processor<T>("subtract");break;
	case 4 : p = new Processor<T>("divide");break;
	default: exit(1);
	}
	
	
	
	(*p) << (*input);
	(*p) >> (*output);
	delete p;
}

