#include"eeValue.h"

eeValue::eeValue(string value): _value(value)
{}


void eeValue::decompile(ostream &os) const
{
    os << fixed << _value;
}
