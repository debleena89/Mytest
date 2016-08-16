#ifndef CONST_EXPR_H
#define CONST_EXPR_H
#include "eeExpr.h"

class eeConstExpr: public eeExpr
{
    private:
        const double _value;

    public:
        eeConstExpr(double value);
        ~eeConstExpr() {}

        double evaluate() const ;
        void decompile(ostream &os) const;
};

#endif
