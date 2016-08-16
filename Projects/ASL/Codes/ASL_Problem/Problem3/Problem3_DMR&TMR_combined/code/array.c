#include <stdio.h>

void main()
{
   int a[10], i;
 __CPROVER_bool OK[15];

  for(i=0;i<10;i++)
   a[i]=0;
   //a[5]=5;
 

  

  for(i=0; i<10; i++)
{
      OK[1] = i>=0 && i<10;
      __CPROVER_assert(!OK[1] || (a[i]==0), " ok fails");
}


}
