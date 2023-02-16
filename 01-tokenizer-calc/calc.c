#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int read_token();

int main()
{
  while(1)
  {
    read_token();
  }
}

int read_token()
{
  int c;
  while((c = getchar()) != EOF)
  {
    switch(c) {
      case '+': case '-':
      case '*': case '/':
        fprintf(stderr, "[OP %c]", c);
        return c;
      case '(': case ')':
        fprintf(stderr, "[POP %c]", c);
        return c;
      case ' ': case '\r':
        while((c = getchar()) == ' ') {}
        ungetc(c, stdin);
        fprintf(stderr, "[WS]");
        return c;
      case '\n':
        fprintf(stderr, "[END]\n");
        exit(0);
      default:
        if(isdigit(c))
        {
          fprintf(stderr, "[NUM %c", c);
          while(isdigit(c = getchar()))
            fprintf(stderr, "%c", c);
          ungetc(c, stdin);
          fprintf(stderr, "]");
          return c;
        }
        else
        {
          fprintf(stderr, "ERR: UNKNOWN TOKEN {%c}\n", c);
          exit(1);
        }
    }
  }
  exit(0);
}
