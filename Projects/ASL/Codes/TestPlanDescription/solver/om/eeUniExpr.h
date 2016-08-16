#ifndef UNI_EXPR_H
#define UNI_EXPR_H

#include "eeExpr.h"

class eeUniExpr: public eeExpr
{
    private:
        eeOperator _operator;
        eeExpr *_operand1;
       
        

    public:
        eeUniExpr(eeOperator op, eeExpr* expr);
        ~eeUniExpr();

        void decompile(ostream &os) const;

};

#endif
