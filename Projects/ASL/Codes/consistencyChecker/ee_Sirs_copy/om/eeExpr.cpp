#include "eeExpr.h"

ostream &operator<<(ostream &os, eeOperator op)
{
    switch(op)
    {
        case EEOP_PLUS:
        {
            os << '+';
            break;
        }
        case EEOP_MINUS:
        {
            os << '-';
            break;
        }
        case EEOP_MUL:
        {
            os << '*';
            break;
        }
        case EEOP_DIV:
        {
            os << '/';
            break;
        }
        case EEOP_UPLUS:
        {
            os << '+';
            break;
        }
        case EEOP_UMINUS:
        {
            os << '-';
            break;
        }
        case EEOP_EQ:
        {
            os << "==";
            break;
        }
        case EEOP_NE:
        {
             os << "!=";
             break;  
        }
        case TERNOP:
        {
            os << '?' ;
            break;
        }
        
        default: assert(0);
    }
    return os;
}

ostream& operator<<(ostream &os, eeExpr *expr)
{
        expr->decompile(os);
            return os;
}

