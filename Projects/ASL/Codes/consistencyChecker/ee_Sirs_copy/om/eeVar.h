#ifndef VAR_H
#define VAR_H

#include "eeDefines.h"

class eeVar
{
    private:
        const string _name;
        double _value;

    public:
        eeVar(const string &name, double value = 0):
            _name(name), _value(value)
        {}
        ~eeVar() {}

        string getName() const
        { return _name; }

        double getValue() const
        { return _value; }

        void setValue(double value)
        { _value = value; }
};

#endif
