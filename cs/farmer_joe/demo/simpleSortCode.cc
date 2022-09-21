
#include <simpleSort.h>

int compare(const void* x, const void* y)
{
 return(*(int*)x>*(int*)y ? 1 : (*(int*)x<*(int*)y ? -1 : 0) );
}

void simpleSort::process()
{
 ACG		gen(time(NULL));
 RandomInteger	rnd(1,LONG_MAX,&gen);
 int		count=NumElements;

 while(--count)
  contents[count]=(unsigned)rnd.asInt();
 qsort((void*)contents,NumElements,sizeof(unsigned),compare);
 sleep(20);
}

bool simpleSort::correctlyProcessed()
{
 bool correct=true;

 for(int i=1;i<NumElements;i++)
   if (contents[i-1]>contents[i])
     correct=false;

 return(correct);
}

void simpleSort::display()
{
 for(int i=0;i<NumElements;i++)
   cout << contents[i] << ',';
 cout << endl;
}
