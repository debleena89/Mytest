
//__CPROVER_bool OK = 1;
int nondet_int();

void main()
{
    int a;
    int b;
        
    a = nondet_int();
    b = nondet_int();
      
    b = a;

  __Cprover_Assert(a == b);
    

   
 
}
