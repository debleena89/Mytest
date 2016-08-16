#include "eeUniExpr.h"

eeUniExpr::eeUniExpr(eeOperator op, eeExpr *expr)
                     : _operator(op),
                        _operand1(expr)
{}		   

eeUniExpr::~eeUniExpr()
{
    delete _operand1;    
}

void eeUniExpr::decompile(ostream &os) const
{
     
    os << '(' << _operator  << ' ' << _operand1 << ')';
}


