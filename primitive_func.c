#include "dior.h"

/***
    Yixin Luo
    2016/4/30
    原始函数
**/
/**
   加法操作 +
**/
atom *padd(atom *args, atom *env) {
  int line = args->position; //获取当前计算发生的行数
  args = CDR(args);
  atom *each;
  long sum = 0;
  while (args != NULL) {
    each = CAR(args);
    atom *tmp = (atom *)eval(each, env); //执行每一个参数
    if (IS(NUMBER, tmp)) {
      sum += GET_VALUE(NUMBER, tmp);
    } else {
      ERRORF(each->position, 加法只能用在数字);
    }
    args = CDR(args);
  }
  free_atom(args);
  atom *res;
  MAKE_ATOM(NUMBER, res, sum, line);
  return res;
}

/**
   乘法操作 *
**/
atom *pmul(atom *args, atom *env) {
  int line = args->position; //获取当前计算发生的行数
  args = CDR(args);
  atom *each;
  long sum = 1;
  while (args != NULL) {
    each = CAR(args);
    atom *tmp = (atom *)eval(each, env); //执行每一个参数
    if (IS(NUMBER, tmp)) {
      sum *= GET_VALUE(NUMBER, tmp);
    } else {
      ERRORF(each->position, 乘法只能用在数字);
    }
    args = CDR(args);
  }
  free_atom(args);
  atom *res;
  MAKE_ATOM(NUMBER, res, sum, line);
  return res;
}

/**
   减法操作 -
**/
atom *psub(atom *args, atom *env) {
  int line = args->position; //获取当前计算发生的行数
  atom *first = (atom *)eval(CADR(args), env);
  atom *second = (atom *)eval(CADDR(args), env);
  long sum = 0;
  if (!IS(NUMBER, first)) {
    ERRORF(first->position, 减法只能作用于整数);
  } else if (!IS(NUMBER, second)) {
    ERRORF(second->position, 减法只能作用于整数);
  } else {
    sum = GET_VALUE(NUMBER, first) - GET_VALUE(NUMBER, second);
  }
  free_atom(args);
  atom *res;
  MAKE_ATOM(NUMBER, res, sum, line);
  return res;
}

/**
   除法操作 /
**/
atom *pdiv(atom *args, atom *env) {
  int line = args->position; //获取当前计算发生的行数
  atom *first = (atom *)eval(CADR(args), env);
  atom *second = (atom *)eval(CADDR(args), env);
  long sum = 0;
  if (!IS(NUMBER, first)) {
    ERRORF(first->position, 除法法只能作用于整数);
  } else if (!IS(NUMBER, second)) {
    ERRORF(second->position, 除法法只能作用于整数);
  } else {
    sum = GET_VALUE(NUMBER, first) / GET_VALUE(NUMBER, second);
  }
  free_atom(args);
  atom *res;
  MAKE_ATOM(NUMBER, res, sum, line);
  return res;
}

/**
   是否 = > < >= <=
**/
atom *pequal(atom *args, atom *env, const char *sym) {
  atom *first = (atom *)eval(CADR(args), env);
  atom *second = (atom *)eval(CADDR(args), env);
  int rt = 0;
  if (!IS(NUMBER, first)) {
    ERRORF(first->position, 比较只能作用于整数);
  } else if (!IS(NUMBER, second)) {
    ERRORF(second->position, 比较只能作用于整数);
  } else {
    rt = GET_VALUE(NUMBER, first) - GET_VALUE(NUMBER, second);
  }
  int cmp;
  if ((cmp = strcmp(sym, "=")) == 0) {
    // =
    atom *res;
    if (rt == 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    free_atom(args);
    return res;
  } else if ((cmp = strcmp(sym, ">")) == 0) {
    // >
    atom *res;
    if (rt > 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    free_atom(args);
    return res;
  } else if ((cmp = strcmp(sym, "<")) == 0) {
    // <
    atom *res;
    if (rt < 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    free_atom(args);
    return res;
  } else if ((cmp = strcmp(sym, ">=")) == 0) {
    // >=
    atom *res;
    if (rt >= 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    free_atom(args);
    return res;
  } else if ((cmp = strcmp(sym, "<=")) == 0) {
    // <=
    atom *res;
    if (rt <= 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    free_atom(args);
    return res;
  }
  ERRORF(args->position, 比较符号不正确);
}

/**
   提供给用户的list操作
**/
atom *list(atom *exp, atom *env) {
  atom *list = CDR(exp);
  while (list != NULL) {
    SET_CAR(list, eval(CAR(list), env));
    list = CDR(list);
  }
  return CDR(exp);
}

/**
   提供给用户的cons
**/
atom *cons(atom *exp, atom *env) {
  atom *first = eval(CADR(exp), env);
  atom *second = eval(CADDR(exp), env);
  atom *ret;
  MAKE_PAIR(ret, first, second, exp->position);
  free_atom(exp);
  return ret;
}

/**
   提供给用户的car
**/
atom *car(atom *exp, atom *env) {
  atom *after = eval(CADR(exp), env);
  return CAR(after);
}

/**
   提供给用户的cdr
**/
atom *cdr(atom *exp, atom *env) {
  atom *after = eval(CADR(exp), env);
  return CDR(after);
}

/**
   append
**/
atom *append(atom *exp, atom *env) {
  atom *first = eval(CADR(exp), env);
  atom *ret = first;
  atom *second = eval(CADDR(exp), env);
  while (IS(PAIR, first) && CDR(first) != NULL) {
    first = CDR(first);
  }
  if (CDR(first) != NULL) {
    ERRORF(exp->position, append只能作用于list类型);
  }
  SET_CDR(first, second);
  return ret;
}

/**
   提供给用户的打印操作
**/
atom *pprint(atom *exp, atom *env) {
  atom *after = eval(CADR(exp), env);
  print_atom(after);
  atom *ret;
  MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  return ret;
}

atom *pprintln(atom *exp, atom *env) {
  atom *after = eval(CADR(exp), env);
  print_atom(after);
  printf("\n");
  atom *ret;
  MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  return ret;
}

/**
   quote
**/
atom *quote(atom *exp, atom *env) {
  return CADR(exp);
}

/**
   引入外部文件
**/
atom *require(atom *exp, atom *env) {
  atom *filename = CADR(exp);
  if (!IS(STRING, filename)) {
    ERRORF(exp->position, REQUIRE的参数不是文件名);
  }
  clean_lex();//清理垃圾
  FILE *file = file_test(GET_VALUE(STRING, filename));
  lexer(file);
  atom *ast = parser();
  eval_sequence(ast, env);
  fclose(file);
  //free_atom(ast);
  atom *ret;
  MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  return ret;
}

/**
   赋值car
**/
atom *set_car(atom *exp, atom *env) {
  atom *list = eval(CADR(exp), env);
  atom *val = eval(CADDR(exp), env);
  //printf("t : %d\n", list->type);
  if (!IS(PAIR, list)) {
    ERRORF(exp->position, SET-CAR只能作用于list类型);
  }
  SET_CAR(list, val);
  atom *ret;
  MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  return ret;
}

/**
   赋值cdr
**/
atom *set_cdr(atom *exp, atom *env) {
  atom *list = eval(CADR(exp), env);
  atom *val = eval(CADDR(exp), env);
  //printf("t : %d\n", list->type);
  if (!IS(PAIR, list)) {
    ERRORF(exp->position, SET-CAR只能作用于list类型);
  }
  SET_CDR(list, val);
  atom *ret;
  MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  return ret;
}
/**
   类型判断
**/
atom *is_type(atom *exp, atom *env, atom_type type) {
  atom *ret;
  if (IS(type, CADR(exp))) {
    MAKE_ATOM(BOOLEAN, ret, 1, exp->position);
  } else {
    MAKE_ATOM(BOOLEAN, ret, 0, exp->position);
  }
  return ret;
}
