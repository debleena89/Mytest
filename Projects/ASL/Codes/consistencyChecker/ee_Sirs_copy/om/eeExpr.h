#ifndef EXPR_H
#define EXPR_H

#include "eeDefines.h"

class eeExpr
{
    public:
        eeExpr() {}
        virtual ~eeExpr() {}

        virtual double evaluate() const = 0;
        virtual void decompile(ostream &os) const = 0;
};

ostream &operator<<(ostream &os, eeOperator op);
ostream &operator<<(ostream &os, eeExpr *expr);

#endif
