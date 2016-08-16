#include"eeConstExpr.h"

eeConstExpr::eeConstExpr(double value): _value(value)
{}

double eeConstExpr::evaluate() const
{
    return _value;
}

void eeConstExpr::decompile(ostream &os) const
{
    os << fixed << _value;
}

