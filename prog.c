#include<stdio.h>

int main(int argc, char **argv)
{
   int F1, F2, Temp1, Temp2;
   
   F1 = atoi(argv[1]);
   F2 = atoi(argv[2]);
   
     if((F1+F2)>=3)
		  Temp1 = 1;
	 else
		  Temp1 = 0;
   printf("%d ", Temp1);
   
    Temp2 = F1 * F2;
   printf("%d ", Temp2);

	 return 0;
}
