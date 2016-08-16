#include "eeTernaryExpr.h"

eeTernaryExpr::eeTernaryExpr(eeOperator op, eeExpr *expr1, eeExpr *expr2,
        eeExpr *expr3): _operator(op), _operand1(expr1),
                        _operand2(expr2), _operand3(expr3)
{}

eeTernaryExpr::~eeTernaryExpr()
{
    delete _operand1;
    delete _operand2;
    delete _operand3;
}

double eeTernaryExpr::evaluate() const
{
    double value ;
    switch (_operator)
    {
        case TERNOP:
        {
            if(_operand1->evaluate() == 0)
                value = _operand3->evaluate();
            else
               value = _operand2->evaluate();  
            break;    
        }
        default: assert(0);
    }
    return value;
}

void eeTernaryExpr::decompile(ostream &os) const
{
    os  << _operand1 << ' ' << _operator << ' ' << _operand2 <<
        ' ' << ':' << ' ' << _operand3;
}
                                                 
