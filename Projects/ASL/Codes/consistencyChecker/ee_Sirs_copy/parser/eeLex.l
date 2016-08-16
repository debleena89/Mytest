%{

#include <stdlib.h>
#include <string.h>
using namespace std;

#include "eeExpr.h"
#include "eeYacc.hpp"

void yyerror(const char*);

%}

%%

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = (char*) malloc(sizeof(char) * (strlen(yytext) + 1));
    strcpy(yylval.str, yytext);
    return ID;
}

#.*\n ;
[0-9]+(\.[0-9]+)? { 
    yylval.value = atof(yytext);
    return NUMBER;
}

"==" return EQ;
"!=" return NE;

[-+*/()=;?:]  {
    return *yytext;
}

[ \t] ; 

\n\r? yylineno++;

. return ERROR;

%%

int yywrap()
{
    return 1;
}

void yyerror(const char *str)
{
    cout << "Error[" << yylineno << "]: " << str << " ..." << endl;
}

