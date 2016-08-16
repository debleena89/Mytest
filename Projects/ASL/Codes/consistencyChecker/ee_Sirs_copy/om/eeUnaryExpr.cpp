#include "eeUnaryExpr.h"

eeUnaryExpr::eeUnaryExpr(eeOperator op, eeExpr *expr):
     _operator(op), _operand(expr)
{}

eeUnaryExpr::~eeUnaryExpr()
{
    delete _operand;
}

double eeUnaryExpr::evaluate() const
{
    double value = _operand->evaluate();
    switch (_operator)
    {
        case EEOP_UPLUS:
        {
            value = + value;
        }
        case EEOP_UMINUS:
        {
            value = - value;
        }
        default: assert(0);
    }
    return value;
}

void  eeUnaryExpr::decompile(ostream &os) const
{
    os << '(' << _operator << _operand << ')';
}   

