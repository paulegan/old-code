/* Simple Tester; works like assert(ex) but keeps going!  SF.06/02/96	*/


#define _mkstr(ex)  #ex

# ifndef NDEBUG
#define _test(ex) 	{if (ex) {printf("%s:%i test succeeded: %s\n", __FILE__, __LINE__, _mkstr(ex));}  else   {printf("%s:%i test failed: %s\n" ,__FILE__, __LINE__, _mkstr(ex));}}
# define test(ex)    _test(ex)
# else
# define _test(ex)
# define test(ex)
# endif

