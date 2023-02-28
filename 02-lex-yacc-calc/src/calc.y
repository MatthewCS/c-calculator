%{
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./src/lib/const.h"
#include "./src/lib/hashset.h"
#include "./src/lib/hashtable.h"

extern FILE* yyin;
FILE* ptrout;

int mode = 0;
HS* reserved_words;
HT* variable_table;

void cliprompt();
void clihelp();
void cliexit();
void openio(char* ifpath, char* ofpath);
int yyparse();
int yylex();
int yyerror(char* message);
%}

%union {  // TOKEN ATTRIBUTES
long double ldval;  // numeric value
char* vname;        // variable name
int opval;          // operator value
}
%start input
%token NEWLINE
%token HELP
%token EXIT
%token <ldval> NUM
%token <vname> VARIABLE
%left <opval> ADDOP
%left <opval> MULOP

%type <ldval> declare // variable declaration
%type <ldval> expr    // expression
%type <ldval> numval  // numeric value
%type <ldval> varval  // variable value

%%
input: line input | line;

line: declare NEWLINE { if(mode == CLIMODE) fprintf(stderr, "  = %Lf\n\n", $1); cliprompt(); }
    | expr NEWLINE    { if(mode == CLIMODE) fprintf(stderr, "  = %Lf\n\n", $1); cliprompt(); }
    | HELP NEWLINE    { clihelp(); cliprompt(); }
    | EXIT NEWLINE    { cliexit(); }
    ;

expr: expr ADDOP expr
    {
      if($2 == '+') $$ = $1 + $3;
      else          $$ = $1 - $3;
    }
    | expr MULOP expr
    {
      if($2 == '*') $$ = $1 * $3;
      else          $$ = $1 / $3;
    }
    | '(' expr ')'  { $$ = $2; }
    | numval        { $$ = $1; }
    | varval        { $$ = $1; }
    ;

declare: VARIABLE '=' expr
      {
        // is this variable a reserved keyword?
        if(hashset_find(reserved_words, $1) == 1)
        {
          fprintf(stderr, "ERROR: Variable \"%s\" is a reserved word. Aborting. . .\n", $1);
          exit(1);
        }

        hashtable_upsert(variable_table, $1, $3);
        $$ = $3;
      }
      ;

numval: NUM         { $$ = $1; }
      | ADDOP NUM
      {
        if($2 == '+') $$ = $2;
        else          $$ = -1 * $2;
      }
      ;

varval: VARIABLE
      {
        // is this variable a reserved keyword?
        if(hashset_find(reserved_words, $1) == 1)
        {
          fprintf(stderr, "ERROR: Variable \"%s\" is a reserved word. Aborting. . .\n", $1);
          exit(1);
        }

        HTfindres result = hashtable_find(variable_table, $1);

        if(result.error == 1)
        {
          fprintf(stderr, "ERROR: Variable \"%s\" not found. Aborting. . .\n", $1);
          exit(1);
        }

        $$ = result.value;
      }
      ;
%%

int main(int argc, char** argv)
{
  // initialize hashset of reserved words
  reserved_words = new_hashset_const_items(RESERVED_WORDS, RESERVED_WORD_COUNT);
  // initialize hashtable of variables
  variable_table = new_hashtable();
  // assume CLI mode until told otherwise
  mode = CLIMODE;

  // CLI mode
  if(argc == 1)
  {
    cliprompt();
    yyparse();

    return 0;
  }

  // we want to compile now
  char* ifpath = NULL;    // input file path
  char* ofpath = NULL;    // output file path
  int modeknown = 0;      // have we already been told what mode we want?
  int c;                  // option value from getopt

  mode = ASMMODE;         // assume we want ASM mode

  while((c = getopt(argc, argv, "i:o:hatc")) != -1)
  {
    switch(c)
    {
      case 'h':     // help message
        fprintf(stderr, "%s", HELPMSG);
        exit(0);
      case 'a':     // ASM mode
        if(modeknown == 1)
        {
          fprintf(stderr, "ERROR: Cannot specify a compile mode multiple times.\n");
          exit(1);
        }

        modeknown = 1;
        mode = ASMMODE;
        break;
      case 't':     // TXT mode
        if(modeknown == 1)
        {
          fprintf(stderr, "ERROR: Cannot specify a compile mode multiple times.\n");
          exit(1);
        }

        modeknown = 1;
        mode = TXTMODE;
        break;
      case 'c':     // TXT mode
        if(modeknown == 1)
        {
          fprintf(stderr, "ERROR: Cannot specify a compile mode multiple times.\n");
          exit(1);
        }

        modeknown = 1;
        mode = CLITXTMODE;
        break;
      case 'i':     // input file
        ifpath = optarg;
        break;
      case 'o':     // output file
        ofpath = optarg;
        break;
      case '?':     // unknown or invalid option
        if(optopt == 'i' || optopt == 'o')
          fprintf(
            stderr,
            "ERROR: Flag -%c requires a filepath to be specified.\n",
            optopt
          );
        else
          fprintf(stderr, "ERROR: Unsupported flag -%c.\n", optopt);
        exit(1);
      default:      // something has gone horribly wrong, abort
        fprintf(stderr, "Unknown error while parsing flags. Aborting.\n");
        exit(1);
    }
  }

  // check to ensure that, if we are in CLITXTMODE, we have no output path
  if(mode == CLITXTMODE && ofpath)
  {
    fprintf(
      stderr,
      "ERROR: Output path must not be given with the -c flag.\n"
    );
    exit(1);
  }
  // check to see if we got both an input path and an output path
  if(!ifpath || (mode != CLITXTMODE && !ofpath))
  {
    fprintf(
      stderr,
      "ERROR: Input path and output path must be specified with -i and -o flags.\n"
    );
    exit(1);
  }
  // check to ensure that the two paths are not identical
  if(mode != CLITXTMODE && strcmp(ifpath, ofpath) == 0)
  {
    fprintf(
      stderr,
      "ERROR: Input path and output path must not be the same.\n"
    );
    exit(1);
  }

  // everything is good to go, let's compile
  //   ... later.

  if(mode == ASMMODE)
  {
    openio(ifpath, ofpath);
    yyparse();
    fprintf(stderr, "ERROR: ASMMODE not yet supported!\n");
    return 0;
  }
  else if(mode == TXTMODE)
  {
    openio(ifpath, ofpath);
    yyparse();

    // iterate over variable table and dump the contents
    // into the output file
    for(int i = 0; i < variable_table->capacity; ++i)
    {
      HTbucket* bucket = variable_table->items[i];

      while(bucket != NULL)
      {
        fprintf(ptrout, "%s=%Lf\n", bucket->node->key, bucket->node->value);
        bucket = bucket->next;
      }
    }

    return 0;
  }
  else if(mode == CLITXTMODE)
  {
    yyin = fopen(ifpath, "r");
    // if we failed to open yyin, return an error
    if(errno != 0)
    {
      fprintf(
        stderr,
        "ERROR OPENING INPUT FILE \"%s\": %s\n",
        ifpath, strerror(errno)
      );
      exit(1);
    }
    yyparse();

    // iterate over variable table and dump the contents
    // into the output file
    for(int i = 0; i < variable_table->capacity; ++i)
    {
      HTbucket* bucket = variable_table->items[i];

      while(bucket != NULL)
      {
        fprintf(stderr, "%s=%Lf\n", bucket->node->key, bucket->node->value);
        bucket = bucket->next;
      }
    }

    return 0;
  }
  return 0;
}

void cliprompt()
{
  if(mode == CLIMODE)
    fprintf(stderr, "> ");
}

void clihelp()
{
  if(mode == CLIMODE)
    fprintf(stderr, "%s\n", HELPMSG);
  else
  {
    fprintf(stderr, "ERROR: Help command only allowed in CLI mode.\n");
    exit(1);
  }
}

void cliexit()
{
  if(mode == CLIMODE)
  {
    fprintf(stderr, "Goodbye!\n");
    exit(0);
  }
  else
  {
    fprintf(stderr, "ERROR: Exit command only allowed in CLI mode.\n");
    exit(1);
  }
}

void openio(char* ifpath, char* ofpath)
{
  yyin = fopen(ifpath, "r");
  // if we failed to open yyin, return an error
  if(errno != 0)
  {
    fprintf(
      stderr,
      "ERROR OPENING INPUT FILE \"%s\": %s\n",
      ifpath, strerror(errno)
    );
    exit(1);
  }

  // pointer to output file
  ptrout = fopen(ofpath, "w");
  // if we failed to open ptrout, return an error
  if(errno != 0)
  {
    fprintf(
      stderr,
      "ERROR OPENING OUTPUT FILE \"%s\": %s\n",
      ofpath, strerror(errno)
    );
    exit(1);
  }
}
