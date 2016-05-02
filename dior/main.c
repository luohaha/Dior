#include "dior.h"

/**
   打印结果
**/
void print_atom(atom *exp) {
  if (exp == NULL)
    return;
  if (IS(BOOLEAN, exp)) {
    if (GET_VALUE(BOOLEAN, exp) == 1)
      printf("#t ");
    else
      printf("#f ");
  } else if (IS(NUMBER, exp)) {
    printf("%ld ", GET_VALUE(NUMBER, exp));
  } else if (IS(STRING, exp)) {
    printf("\"%s\" ", GET_VALUE(STRING, exp));
  } else if (IS(CHARACTER, exp)) {
    printf("'%c' ", GET_VALUE(CHARACTER, exp));
  } else if (IS(SYMBOL, exp)) {
    printf("%s ", GET_VALUE(SYMBOL, exp));
  } else if (IS(PAIR, exp)){
    printf("( ");
    while (exp != NULL) {
      if (IS(PAIR, exp)) {
	print_atom(CAR(exp));
      } else {
	printf(". ");
	print_atom(exp);
      }
      exp = CDR(exp);
    }
    printf(") ");
  } else if (IS(FUNCTION, exp)) {
    printf("#func ");
  }
}

/**
   测试文件合法性
**/
FILE *file_test(char *filename) {
  char *back = strrchr(filename, '.');
  //printf("%s\n", back);
  int cmp;
  if (back == NULL || (cmp = strcmp(back+1, "dior")) != 0) {
    //后缀名不正确
    printf("this file must be xxx.dior\n");
    exit(-1);
  }
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("%s is not exit!\n", filename);
    exit(-1);
  } else
    return file;
}

/**
   清理词法分析器，留下的垃圾
**/
void clean_lex() {
  lex_index = 0;
}

int main(int argc, char *args[]) {
  if (argc != 2) {
    printf("usage is : ./dior [filename]\n");
    return 0;
  }
  FILE *file = file_test(args[1]);
  lexer(file);
  atom *ast = parser();
  atom *env = init_env();
  atom *ret = eval_sequence(ast, env);
  print_atom(ret);
  printf("\n");
  fclose(file);
  return 0;
}
