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
    return res;
  } else if ((cmp = strcmp(sym, ">")) == 0) {
    // >
    atom *res;
    if (rt > 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    return res;
  } else if ((cmp = strcmp(sym, "<")) == 0) {
    // <
    atom *res;
    if (rt < 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    return res;
  } else if ((cmp = strcmp(sym, ">=")) == 0) {
    // >=
    atom *res;
    if (rt >= 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    return res;
  } else if ((cmp = strcmp(sym, "<=")) == 0) {
    // <=
    atom *res;
    if (rt <= 0) {
      MAKE_ATOM(BOOLEAN, res, 1, args->position);
    } else {
      MAKE_ATOM(BOOLEAN, res, 0, args->position);
    }
    return res;
  }
  ERRORF(args->position, 比较符号不正确);
}

