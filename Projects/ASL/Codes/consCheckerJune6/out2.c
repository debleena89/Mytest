#include <stdio.h>

int main()
{

int x, z;
x = 2147483647;
z = -1879048194;
//__CPROVER_assume(z < 8.0);
printf("%d",x+2);
if(z > (2+x))
	printf("sorry");

//__CPROVER_assert(0, "Hello");
return 0;
}
