nclude<iostream>
#include<z3++.h>
using namespace z3;

int main() {
	    context c;
		    expr x = c.int_const("x");
			    std::cout << x + 1 << "\n";
				    return 0;
}
