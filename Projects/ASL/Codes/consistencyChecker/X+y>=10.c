#include <stdio.h>

int main()
{
int x, y, z;
x = 1073745921;
y = 1073741830;
z = x + y;
printf("%d",z);
if(z < 10)
printf("less than 10");
else
printf("not that")	;
return 0;
}
