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
    EEOP_EQ,
    EEOP_NE,
    TERNOP
};
#endif
