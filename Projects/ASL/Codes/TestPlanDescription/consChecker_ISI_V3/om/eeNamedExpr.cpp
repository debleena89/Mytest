#include"eeNamedExpr.h"

eeNamedExpr::eeNamedExpr(const eeVar *var): _var(var)
{}

void eeNamedExpr::decompile(ostream &os) const
{
    os << _var->getName();
}

