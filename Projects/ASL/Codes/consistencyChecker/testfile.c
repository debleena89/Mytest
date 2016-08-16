#include <stdio.h>

int main()
{

int  a;

a = 8;
__CPROVER_assume(!a);
__CPROVER_assert(0, "Hello");
return 0;
}
