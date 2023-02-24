%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "calc.tab.h"

int yylex();
int yyerror(char* message);
static void yyunput(int c, char* yy_bp);
static int input(void);
%}

number ([0-9]*\.[0-9]+|[0-9]+\.?)
infinity (inf)
nan (nan|NaN)
whitespace [ \t]
help (help|Help|HELP)
exit (exit|Exit|EXIT)
variable ([a-zA-Z_][a-zA-Z0-9_]*)

%%
"+"             { yylval.opval = '+'; return ADDOP; }
"-"             { yylval.opval = '-'; return ADDOP; }
"*"             { yylval.opval = '*'; return MULOP; }
"/"             { yylval.opval = '/'; return MULOP; }
"("             { return yytext[0]; }
")"             { return yytext[0]; }
{number}        {
                  yylval.ldval = strtold(yytext, NULL);
                  return NUM;
                }
{infinity}      {
                  yylval.ldval = strtold(yytext, NULL);
                  return NUM;
                }
{nan}           {
                  yylval.ldval = strtold(yytext, NULL);
                  return NUM;
                }
{whitespace}    { ; }
"\n"            { return NEWLINE; }
{help}          { return HELP; }
{exit}          { return EXIT; }
{variable}      { yylval.vname = strdup(yytext); return VARIABLE; }
.               { return yytext[0]; }
%%

int yyerror(char* message)
{
  fprintf(stderr, "LEX ERROR: \"%s\"\n", message);
  return 0;
}
