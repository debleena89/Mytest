
//__CPROVER_bool OK = 1;
int nondet_int();

int equal(int* a, int* b, int n)
{
	int i;

	for ( i = 0; i < n; i++)
	{
           
	    if (a[i] != b[i])
		return 0;

                 
	}
	return 1;
}

verifequal(int* a, int* b, int n, __CPROVER_bool retn)
{
    int i;
    __CPROVER_bool OK = 1; 
   
    for ( i = 0; i < n; i++)
     {
      OK = OK && (a[i] == b[i]);     
     }

    __CPROVER_assert (!OK || (retn == 1), "Hey OK1 fails");
}

void main()
{
    int Max = nondet_int(); // array bounds are defined non deterministically
    int a[Max] ;
    int b[Max] ;
    int i;

    //int limit = nondet_int(); // n is also defined non-deterministically
   
    for(i = 0; i <Max; i++) 
      {
        a[i] = nondet_int();
        b[i] = nondet_int();
      }
  
    __CPROVER_bool value;

    value =  equal(a, b, Max);
    verifequal(a, b, Max, value); 
 
}
