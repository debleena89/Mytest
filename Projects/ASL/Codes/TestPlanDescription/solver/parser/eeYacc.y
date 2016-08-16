%{

#include <map>
#include <cstdlib>
using namespace std;

#include <string.h>
#include "eeDefines.h"
#include "eeVar.h"
#include "eeExpr.h"
#include "eeNamedExpr.h"
#include "eeValue.h"
#include "eeBinaryExpr.h"
#include "eeUniExpr.h"


int yylex();
void yyerror(const char*);
extern FILE *yyout;
static bool debug = false;
static string INT = "int", CHAR = "char", BOOL = "bool", DOUBLE = "double", TYPE;
static bool intflag = 0, charflag = 0, boolflag = 0, doubleflag = 0;
map<string, eeVar*> varSymTab;

%}

%union
{
    char *str;
    double value;
    eeExpr *expr;
    eeVar *idlist;
}

%token <str> ID
%token <str> KWD
%token <str> STMT
%token <str> NUMBER
%token <str> BOOLVALUE
%token ERROR
%type <expr> sequence
%type <expr> check

%left AND OR LE GE '<' '>''=' NOT
%left EQ NE '?' '-' '+'
%left '*' '/'
%nonassoc UMINUS

%start program

%%

program: program VarDeclStmt | VarDeclStmt | program Stmt | Stmt;

VarDeclStmt: KWD { if(strcmp($1,"char") == 0) charflag = 1;
            else if(strcmp($1,"int") == 0) intflag = 1;
            else if(strcmp($1,"bool") == 0) boolflag = 1;
            else if(strcmp($1,"double") == 0) doubleflag = 1;} 
       idlist  ';'
     {
         charflag = 0;
         intflag = 0;
         boolflag = 0;
         doubleflag = 0;
     }
Stmt:ID '=' sequence ':' check';'    
      {
        cout << " changed";
        //$1->decompile(std::cout);
        cout<<")"<<endl;  
      }

sequence: 
    /*expr EQ expr
    {
       $$ = new eeBinaryExpr(EEOP_EQ, $1, $3);  
    }
    | expr NE expr
    {
        $$ = new eeBinaryExpr(EEOP_NE, $1, $3); 
    }
    |NOT expr
    {
      $$ = new eeUniExpr(EEOP_NOT, $2);
    }
    | '(' expr ')'
    {
        $$ = $2;
    }
    | BOOLVALUE
    {
       $$ = new eeValue($1);     
    }
    | NUMBER
    {         
       $$ = new eeValue($1);       
    }*/
    '[' sequence ']' 
     {

      cout << "I entered";
     }
     |
      ID
     {
       if (varSymTab.find($1) == varSymTab.end())
        { 
            if (debug)
                cout << "> creating variable: " << $1 << endl;
            
        yyerror($1);
        }
        eeVar *var = varSymTab[$1];
        $$ = new eeNamedExpr(var);
        TYPE = var->getType();

        if(TYPE == "char") charflag = 1;
        else if(TYPE == "int") intflag = 1;
        else if(TYPE == "bool") {boolflag = 1;yyout = fopen("output", "w"); fprintf(yyout,"%c %c",'0','1'); }
        else if(TYPE == "double") doubleflag = 1;
        

        
        free($1);
     }

check:

 ID
    {
       if (varSymTab.find($1) == varSymTab.end())
        { 
            if (debug)
                cout << "> creating variable: " << $1 << endl;
            
        yyerror($1);
        }
        eeVar *var = varSymTab[$1];
        $$ = new eeNamedExpr(var);
        TYPE = var->getType();

        if(TYPE == "char") charflag = 1;
        else if(TYPE == "int") intflag = 1;
        else if(TYPE == "bool") boolflag = 1;
        else if(TYPE == "double") doubleflag = 1;
        
        free($1);
    }
    
 idlist: idlist ',' idlist
       { 
       }
       | ID 
       {
        if (varSymTab.find($1) == varSymTab.end())
        {
            if (debug)
                cout << "> creating variable: " << $1 << endl;
            varSymTab[$1] = new eeVar($1);
            eeVar *vardefine = varSymTab[$1];

            if(charflag == 1)
             vardefine->setType(CHAR);
            else if(intflag == 1)
             vardefine->setType(INT);
            else if(boolflag == 1)
             vardefine->setType(BOOL);
            else if(doubleflag == 1)
             vardefine->setType(DOUBLE);

            vardefine->decompiletoyices(std::cout); 
            
        }
       else
        {
           yyerror($1);
        }
       free($1);
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
