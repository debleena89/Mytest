#ifndef UNARY_EXPR_H
#define UNARY_EXPR_H

#include "eeExpr.h"

class eeUnaryExpr: public eeExpr
{
    private:
        eeOperator _operator;
        eeExpr *_operand;

    public:
        eeUnaryExpr(eeOperator op, eeExpr *expr);
        ~eeUnaryExpr();

        double evaluate() const;
        void decompile(ostream &os) const;
};

#endif
