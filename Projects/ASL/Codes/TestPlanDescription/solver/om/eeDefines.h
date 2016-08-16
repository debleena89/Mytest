#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

enum eeOperator {
    EEOP_UPLUS,
    EEOP_UMINUS,
    EEOP_PLUS,
    EEOP_MINUS,
    EEOP_MUL,
    EEOP_DIV,
    EEOP_AND,
    EEOP_OR,
    EEOP_LE,
    EEOP_LT,
    EEOP_GE,
    EEOP_GT,
    EEOP_IQ,
    EEOP_EQ,
    EEOP_NE,
    EEOP_NOT,
    TERNOP
};
#endif
