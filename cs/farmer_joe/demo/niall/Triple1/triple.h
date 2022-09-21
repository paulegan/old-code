#include <String.h>
#include <Processor.h>

#define IfCompleteExecute if(input&&output&&function)execute()

template <class T>
class Triple{
public:
	Triple() 	      { input = output = NULL; function = 0; };
	~Triple() 	      { };
	void setInput(T*);
	void setOutput(T*);
	void setFunction(int);
private:
	T* input;
	int function;
	T* output;
	Processor<T>* p;
	void execute(void);
};
