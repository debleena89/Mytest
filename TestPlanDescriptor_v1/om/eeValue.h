#ifndef Number_H
#define Number_H
#include "eeExpr.h"

class eeValue: public eeExpr
{
    private:
         string _value;
              
    public:
        eeValue(string value);
        ~eeValue() {}

        void decompile(ostream &os) const;
};

#endif
