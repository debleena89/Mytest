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

double eeBinaryExpr::evaluate() const
{
    double value;
    switch (_operator)
    {
        case EEOP_PLUS:
        {
            value = _operand1->evaluate() + _operand2->evaluate();
            break;
        }
        case EEOP_MINUS:
        {
            value = _operand1->evaluate() - _operand2->evaluate();
            break;
        }
        case EEOP_MUL:
        {
            value = _operand1->evaluate() * _operand2->evaluate();
            break;
        }
        case EEOP_DIV:
        {
            value = _operand1->evaluate() / _operand2->evaluate();
            break;
        }
        case EEOP_EQ:
        {
            value = (_operand1->evaluate() == _operand2->evaluate());
            break;
        }
        case EEOP_NE:
        {
            value = (_operand1->evaluate() != _operand2->evaluate());
            break;
        }
        default: assert(0);
    }
    return value;
}

void eeBinaryExpr::decompile(ostream &os) const
{
    os << '(' << _operand1 << ' ' << _operator << ' ' << _operand2 << ')';
}
