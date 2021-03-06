double nondet_double();
double x;

void oprn(double a, double b, double c, double d)
{
//   	x= ((a*b) - (c*d)); 	// condition1
	x = a*b;	 	// condition2 ---for this condition, there is no convergence to the solution
//	x = a-b;         	// condition3
}

void main()
{
	double a, b, c, d;
    
	a = nondet_double();
	b = nondet_double();
	c = nondet_double();
	d = nondet_double();
    
	__CPROVER_assume(__CPROVER_isfinited(a));
	__CPROVER_assume(__CPROVER_isfinited(b));
	__CPROVER_assume(__CPROVER_isfinited(c));
	__CPROVER_assume(__CPROVER_isfinited(d));
	
	//__CPROVER_assume( a > 0 && a < 100 );
	//__CPROVER_assume( b > 10 && b < 100);
	
	oprn(a, b, c, d);

    	//__CPROVER_assert (x ==  ((a*b)- (c*d)), "Hey fails"); // assert for condition1
	__CPROVER_assert (x ==  a*b, "Hey fails");		// assert for condition2 
	//__CPROVER_assert (x ==  a-b, "Hey fails");		// assert for condition3
}
