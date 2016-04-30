#include "dior.h"

void PP(atom *car) {
  if (car == NULL)
    printf("pp is null\n");
  //while (y != NULL) {
  //atom *car = CAR(y);
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
      //PP(car);
      printf("pair\n");
    } else {
      printf("error\n");
    }
    //y = CDR(y);
    //}
}

int main() {
  lexer(stdin);
  /*
  int i;
  for (i = 0; i < lex_index; i++) {
    printf("%s,  %d\n", lex_list[i], lex_list_row[i]);
    }*/
  /*
  lex_list[0] = "(";
  lex_list[1] = "(";
  lex_list[2] = "(";
  lex_list[3] = "lambda";
  lex_list[4] = "(";
  lex_list[5] = "x";
  lex_list[6] = ")";
  lex_list[7] = "(";
  lex_list[8] = "*";
  lex_list[9] = "x";
  lex_list[10] = "x";
  lex_list[11] = ")";
  lex_list[12] = ")";
  lex_list[13] = "2";
  lex_list[14] = ")";
  lex_list[15] = ")";
  */
  parser();
  //if (ast != NULL)
  //PP(ast);
  atom *env = init_env();
  atom *ret = eval_sequence(ast, env);
  PP(ret);
  return 0;
}
