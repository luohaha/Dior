#include "dior.h"

int main() {
    lexer(stdin);
    int i;
    for (i = 0; i < lex_index; i++) {
      printf("%s,  %d\n", lex_list[i], lex_list_row[i]);
    }
  return 0;
}
