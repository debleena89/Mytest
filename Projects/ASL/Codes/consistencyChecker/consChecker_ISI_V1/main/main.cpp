#include <iostream>
#include <cstdio>
using namespace std;

extern FILE *yyin;
int yyparse();
void yyfinalize();

int parse(const char *path = NULL)
{
    if (path == NULL)
        yyin = stdin;
    else
        yyin = fopen(path, "r");
    if (yyin == NULL)
    {
        cout << "Error: Can't open file " << path << " ..." << endl;
        return 2;
    }
    if (yyparse() != 0)
        return 1;
    yyfinalize();
    cout<<"(check)";
    fclose(yyin);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1)
        return parse();
    for (int i = 1; i < argc; i++)
    {
        if (parse(argv[i]) != 0)
            return 1;
    }
    return 0;    
}
