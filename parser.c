#include "dior.h"
/**
   Yixin Luo
   2016/4/27
   语法分析器，根据词法分析器得到的结果，生成AST
**/
//栈，用来检查括号
int si = 0;

atom *ast;
atom *generate(int *i);
/**
   产生list
**/
atom *generate_list(int *i) {
  atom *n = NULL;
  if (lex_list[*i][0] == ')') {
    return NULL;
  } else {
    atom *a = (atom*)generate(i);
    MAKE_PAIR(n, a, generate_list(i), lex_list_row[(*i)-1]);
  }
  return n;
}

/**
   产生boolean
**/
atom *generate_boolean(int i) {
  atom *a;
  if (lex_list[i][0] != '#') {
    ERRORF(lex_list_row[i], 不是boolean类型);
  }
  if (lex_list[i][1] == 't' || lex_list[i][1] == 'T') {
    MAKE_ATOM(BOOLEAN, a, 1, lex_list_row[i]);
  } else if (lex_list[i][1] == 'f' || lex_list[i][1] == 'F') {
    MAKE_ATOM(BOOLEAN, a, 0, lex_list_row[i]);
  } else {
    ERRORF(lex_list_row[i], 不是boolean类型);
  }
  return a;
}

/**
   产生number
**/
atom *generate_number(int i) {
  int j = 0;
  long real = 0;
  int sign = 1;
  atom *a;
  if (lex_list[i][j] == '-') {
    sign = -1;
    j++;
  }
  if (lex_list[i][j] == '+') {
    sign = 1;
    j++;
  }
  while (lex_list[i][j] != '\0' && isdigit(lex_list[i][j])) {
    real = (real * 10) + (lex_list[i][j] - '0');
    j++;
  }
  if (lex_list[i][j] != '\0') {
    ERRORF(lex_list_row[i], 数字类型不正确);
  }
  MAKE_ATOM(NUMBER, a, real * sign, lex_list_row[i]);
  return a;
}

/**
   产生string
**/
atom *generate_string(int i) {
  int length = strlen(lex_list[i]);
  char *real = malloc(length-1);
  if (lex_list[i][0] != '"' || lex_list[i][length-1] != '"') {
    ERRORF(lex_list_row[i], 字符串格式不正确);
  }
  //复制字符串,但是不复制""
  memcpy(real, lex_list[i]+1, length - 2);
  real[length-2] = '\0';
  atom *a;
  MAKE_ATOM(STRING, a, real, lex_list_row[i]);
  return a;
}

/**
   产生character
**/
atom *generate_charater(int i) {
  if (lex_list[i][0] != '\'' || lex_list[i][2] != '\'' || strlen(lex_list[i]) != 3) {
    ERRORF(lex_list_row[i], 字符的格式不对);
  }
  atom *a;
  MAKE_ATOM(CHARACTER, a, lex_list[i][1], lex_list_row[i]);
  return a;
}

/**
   产生symbol
**/
atom *generate_symbol(int i) {
  atom *a;
  MAKE_ATOM(SYMBOL, a, lex_list[i], lex_list_row[i]);
  return a;
}

/**
   分发器，决定使用哪一个generate
**/
atom *generate(int *i) {
  atom *a;
  switch (lex_list[*i][0]) {
  case '(':
    (*i)++;
    a = generate_list(i);
    (*i)++;
    return a;
  case '#':
    a = generate_boolean(*i);
    (*i)++;
    return a;
  case '"':
    a = generate_string(*i);
    (*i)++;
    return a;
  case '\'':
    a = generate_charater(*i);
    (*i)++;
    return a;
  default:
    if ((lex_list[*i][0] == '+' || lex_list[*i][0] == '-') && isdigit(lex_list[*i][1])) {
      //有符号数字
      a = generate_number(*i);
      (*i)++;
      return a;
    } else if (isdigit(lex_list[*i][0])) {
      //数字
      a = generate_number(*i);
      (*i)++;
      return a;
    } else {
      //符号symbol
      a = generate_symbol(*i);
      (*i)++;
      return a;
    }
  }
}

void parser() {
  int i = 0; // 当前读取的位置
  ast = generate(&i);
}
