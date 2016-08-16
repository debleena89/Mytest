#ifndef TERNARY_EXPR_H
#define TERNARY_EXPR_H

#include "eeExpr.h"

class eeTernaryExpr: public eeExpr
{
    private:
        eeOperator _operator;
        eeExpr *_operand1;
        eeExpr *_operand2;
        eeExpr *_operand3;

public:
        eeTernaryExpr(eeOperator op, eeExpr* expr1, eeExpr *expr2, 
                      eeExpr *expr3);
        ~eeTernaryExpr();

        double evaluate() const;
        void decompile(ostream &os) const;
};

#endif                           
