#include"eeNamedExpr.h"

eeNamedExpr::eeNamedExpr(const eeVar *var): _var(var)
{}

double eeNamedExpr::evaluate() const
{
    return _var->getValue();

}

void eeNamedExpr::decompile(ostream &os) const
{
    os << _var->getName();
}

