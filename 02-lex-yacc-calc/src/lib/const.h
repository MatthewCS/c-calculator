#ifndef __CONST_H__
#define __CONST_H__

// tokens
static const int T_NUM = 257;
static const int T_VARIABLE = 258;
static const int T_ADDOP = 259;
static const int T_MULOP = 260;

// calc modes
static const int CLIMODE = 1;  // use CLI version of calculator
static const int ASMMODE = 2;  // compile to assembly
static const int TXTMODE = 3;  // compile to plaintext file
static const int CLITXTMODE = 4;   // dump plaintext output to terminal

// program help message
static const char* HELPMSG =
"Simple calculator made using Lex & Yacc\n\
\n\
This calculator can be run in one of two modes: CLI mode and compiler mode.\n\
 - CLI mode is accessed by running the program with no arguments.\n\
    ex: ./calc\n\
 - Compiler mode is accessed by running the program with the -i and -o flags.\n\
    ex: ./calc -i input.calc -o output\n\
\n\
Flags for compiler mode:\n\
  -i  specify an input file.                    REQUIRED\n\
  -o  specify an output file.                   REQUIRED\n\
  -h  display this help message, then exit.     OPTIONAL\n\
  -a  output to assembly. (default, NOT        OPTIONAL\n\
        COMPATIBLE WITH -t FLAG or -c FLAG)\n\
  -t  output to a plaintext file, instead       OPTIONAL\n\
        of assembly. (NOT COMPATIBLE WITH \n\
        -a FLAG or -c FLAG)\n\
  -c  output to the terminal, showing only      OPTIONAL\n\
        the output normally given from the\n\
        -t flag. (NOT COMPATIBLWE WITH -a\n\
        FLAG or -t FLAG\n\
\n\
SUPPORTED COMMANDS:\n\
  - exit, Exit, EXIT                            CLI MODE ONLY\n\
      Exit from CLI mode.\n\
  - help, Help, HELP                            CLI MODE ONLY\n\
      Display this help message.\n\
\n\
SUPPORTED OPERATIONS:\n\
  <num> + <num>         ADDITION\n\
  <num> - <num>         SUBTRACTION\n\
  <num> * <num>         MULTIPLICATION\n\
  <num> / <num>         DIVISION\n\
  (<operation[s]>)      PARENTHESIS\n\
\n\
The calculator supports whole numbers, as well as decimal values.\n\
  Numbers must match the following regex:\n\
    \\([0-9]*\\.[0-9]+|[0-9]+\\.?)\n\
  Numbers can also be lead with a + or - sign.\n\
  The following examples are valid numbers:\n\
  -  123\n\
  -  1.23\n\
  -  .123\n\
  -  +123\n\
  -  -123\n\
  -  01.230\n\
\n\
The calculator supports variables.\n\
  A variable name must not be a reserved word, such as:\n\
    \"help\", \"Exit\", \"RESERVED\"\n\
  A variable must start with a letter or an underscore.\n\
  Variable names may contain numbers after the first character.\n\
  To declare a variable, use:\n\
    <varname> = <expression>\n\
  ex:\n\
  - x = 5\n\
  - new_value = x / 0.5\n\
  - new_value = new_value * 2\n\
  If you attempt to access an undeclared variable in the\n\
  expression section, then the calculator throws an error and\n\
  then exits immediately.\n\
";

// reserved words
static const char* RESERVED_WORDS[] =  {
  // keywords that cannot be used as variable names
  "help", "Help", "HELP",
  "exit", "Exit", "EXIT",
  "reserved", "Reserved", "RESERVED",
  "inf",
  "nan", "NaN"
};
static const int RESERVED_WORD_COUNT = 12;

#endif
