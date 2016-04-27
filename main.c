#include "dior.h"

void PP(atom *y) {
  while (y != NULL) {
    atom *car = CAR(y);
    if (IS(BOOLEAN, car)) {
      printf("%c\n", GET_VALUE(BOOLEAN, car));
    } else if (IS(NUMBER, car)) {
      printf("%ld\n", GET_VALUE(NUMBER, car));
    } else if (IS(STRING, car)) {
      printf("%s\n", GET_VALUE(STRING, car));
    } else if (IS(CHARACTER, car)) {
      printf("%c\n", GET_VALUE(CHARACTER, car));
    } else if (IS(SYMBOL, car)) {
      printf("%s\n", GET_VALUE(SYMBOL, car));
    } else if (IS(PAIR, car)){
      PP(car);
    } else {
      printf("error\n");
    }
    y = CDR(y);
  }
}

int main() {
  lexer(stdin);
  int i;
  for (i = 0; i < lex_index; i++) {
    printf("%s,  %d\n", lex_list[i], lex_list_row[i]);
  }
  /*
  lex_list[0] = "(";
  lex_list[1] = "(";
  lex_list[2] = "2";
  lex_list[3] = ")";
  lex_list[4] = ")";
  */
  parser();
  //if (ast != NULL)
  PP(ast);
  return 0;
}
