#ifndef NAMEDEXPR_H
#define NAMEDEXPR_H

#include "eeExpr.h"
#include "eeVar.h"

class eeNamedExpr: public eeExpr
{
    private:
        const eeVar *_var;

    public:
        eeNamedExpr(const eeVar *var);
        ~eeNamedExpr() {}

        void decompile(ostream &os) const;	
};

#endif
