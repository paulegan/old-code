class twoIntClass{
public:
	twoIntClass()		{};
	~twoIntClass()		{};
 void	add(void) 		{ result = numberOne + numberTwo;};
 void   subtract(void)		{ result = numberOne - numberTwo;};
 void 	multiply(void)		{ result = numberOne * numberTwo;};
 void	divide(void)		{ result = numberOne / numberTwo;};

 int 	result;
 int	numberOne;
 int	numberTwo;
};
