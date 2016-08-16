#ifndef BINARY_EXPR_H
#define BINARY_EXPR_H

#include "eeExpr.h"

class eeBinaryExpr: public eeExpr
{
    private:
        eeOperator _operator;
        eeExpr *_operand1;
        eeExpr *_operand2;

    public:
        eeBinaryExpr(eeOperator op, eeExpr* expr1, eeExpr *expr2);
        ~eeBinaryExpr();

        double evaluate() const;
        void decompile(ostream &os) const;
};

#endif
