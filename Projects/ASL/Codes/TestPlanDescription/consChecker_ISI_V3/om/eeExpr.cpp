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
        case EEOP_AND:
        {
            os << "and";
            break;
        }
        case EEOP_OR:
        {
            os << "or";
            break;
        }
        case EEOP_LE:
        {
          os << "<=";
             break;
        }
        case EEOP_LT:
        {
          os << '<';
             break;
        }
        case EEOP_GE:
        {
          os << ">=";
             break;
        }
        case EEOP_GT:
        {
          os << '>';
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
        case EEOP_IQ:
        {
            os << '=';
            break;
        }
        case EEOP_EQ:
        {
            os << "=";
            break;
        }
        case EEOP_NE:
        {
             os << "/=";
             break;  
        }
         case EEOP_NOT:
        {
             os << "not";
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

