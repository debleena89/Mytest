%{

#include <map>
#include <cstdlib>
using namespace std;

#include "eeDefines.h"
#include "eeVar.h"
#include "eeExpr.h"
#include "eeConstExpr.h"
#include "eeNamedExpr.h"
#include "eeUnaryExpr.h"
#include "eeBinaryExpr.h"
#include "eeTernaryExpr.h"

int yylex();
void yyerror(const char*);

static bool debug = false;
map<string, eeVar*> varSymTab;

%}

%union
{
    char *str;
    double value;
    eeExpr *expr;
}

%token <str> ID
%token <value> NUMBER
%token ERROR
%type <expr> expr

%left EQ NE '?' '-' '+'
%left '*' '/'
%nonassoc UMINUS

%start program

%%

program: program stmt | stmt;

stmt: ID '=' expr ';'
    {
        if (varSymTab.find($1) == varSymTab.end())
        {
            if (debug)
                cout << "> creating variable: " << $1 << endl;
            varSymTab[$1] = new eeVar($1);
        }
        eeVar *var = varSymTab[$1];
        var->setValue($3->evaluate());
        free($1);
        delete $3;
    }
    | expr ';'
    {
        cout << $1 << " : " << fixed << $1->evaluate() << endl;
        delete $1;
    }

expr: expr '+' expr
    {
        $$ = new eeBinaryExpr(EEOP_PLUS, $1, $3);
    }
    | expr '-' expr
    {
        $$ = new eeBinaryExpr(EEOP_MINUS, $1, $3);
    }
    | expr '*' expr
    {
        $$ = new eeBinaryExpr(EEOP_MUL, $1, $3);
    }
    | expr '/' expr
    {
        $$ = new eeBinaryExpr(EEOP_DIV, $1, $3);
    }
    | expr EQ expr
    {
        $$ = new eeBinaryExpr(EEOP_EQ, $1, $3);
    }
    | expr NE expr
    {
        $$ = new eeBinaryExpr(EEOP_NE, $1, $3);
    }
    | expr '?' expr ':' expr
    {
       $$ = new eeTernaryExpr(TERNOP, $1, $3, $5); 
    }
    | NUMBER
    {
        $$ = new eeConstExpr($1);    
    }
    | ID
    {
        if (varSymTab.find($1) == varSymTab.end())
        {
            if (debug)
                cout << "> creating variable: " << $1 << endl;
            varSymTab[$1] = new eeVar($1);
        }
        eeVar *var = varSymTab[$1];
        $$ = new eeNamedExpr(var);
        free($1);
    }
    | '(' expr ')'
    {
        $$ = $2;
    }
    | '-' expr %prec UMINUS
    {
        $$ = new eeUnaryExpr(EEOP_UMINUS, $2);
    }
    ;

%%

void yyfinalize()
{
    if (debug)
        cout << "> Variables created:" << endl;
    map<string, eeVar*>::iterator iter;   
    for(iter = varSymTab.begin(); iter != varSymTab.end(); iter++)
    {
        if (debug)
            cout << iter->first << " : " <<
                fixed << iter->second->getValue() << endl;
        delete iter->second;
    }
    varSymTab.clear();
}
