#include <stdio.h>

void main()
{
	 int x;
	 __CPROVER_assume(x == 10);
	 __CPROVER_assume(x > 0);
	 __CPROVER_assert(1,"Fails");
}
