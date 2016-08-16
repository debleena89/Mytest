#include "eeBinaryExpr.h"

eeBinaryExpr::eeBinaryExpr(eeOperator op, eeExpr *expr1,
        eeExpr *expr2): _operator(op),
                        _operand1(expr1), _operand2(expr2)
{}

eeBinaryExpr::~eeBinaryExpr()
{
    delete _operand1;
    delete _operand2;
}

void eeBinaryExpr::decompile(ostream &os) const
{
     
    os << '(' << _operator  << ' ' << _operand1 << ' ' << _operand2 << ')';
}


