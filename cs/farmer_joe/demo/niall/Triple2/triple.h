#include <String.h>
#include <Processor.h>

#define IfCompleteExecute if(input&&output&&function)execute()


template <class T>
class Triple{
public:
	Triple() 	      { input = output =NULL; function = 0; };
	~Triple() 	      { if( p ) p->~Processor(); };
	void operator=(int f) { function = f; IfCompleteExecute; };
	void setInput(T& t)   { input = &t; IfCompleteExecute; };
	void setOutput(T& t)  { output = &t; IfCompleteExecute; };
	void execute(void);
private:
	T* input;
	int function;
	T* output;
	Processor<T>* p;
	
};
