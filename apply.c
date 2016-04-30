#include "dior.h"

/***
    Yixin Luo
    2016/4/29
    函数展开执行
**/
/**
   判断是否是原始函数
   如果是则执行，并返回
**/
atom *is_primary_func_and_exe(atom *exp, atom *env) {
  if (!IS(SYMBOL, CAR(exp)))
    return NULL;
  atom *res = NULL;
  int cmp = -1;
  if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "+")) == 0) {
    res = padd(exp, env);
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "-")) == 0) {
    res = psub(exp, env);
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "*")) == 0) {
    res = pmul(exp, env);
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "/")) == 0) {
    res = pdiv(exp, env);
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "=")) == 0) {
    res = pequal(exp, env, "=");
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), ">")) == 0) {
    res = pequal(exp, env, ">");
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "<")) == 0) {
    res = pequal(exp, env, "<");
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), ">=")) == 0) {
    res = pequal(exp, env, ">=");
  } else if ((cmp = strcmp(GET_VALUE(SYMBOL, CAR(exp)), "<=")) == 0) {
    res = pequal(exp, env, "<=");
  }
  
  return res;
}
/**
   执行一个atom链表，并返回对应结果的atom链表
   magic!
**/
atom *eval_list_return_list(atom *vars, atom *vals, atom *env) {
  atom *old_vars = vars;
  if ((vars == NULL && vals != NULL) ||
      (vars != NULL && vals == NULL)) {
    ERRORF(vars->position, 函数实际参数数目错误);
  }
  if (vars == NULL)
    return NULL;
  atom *first;
  MAKE_PAIR(first, eval(CAR(vals), env), NULL, -1);
  atom *index = first;
  vars = CDR(vars);
  vals = CDR(vals);
  while (vars != NULL) {
    atom *each;
    MAKE_PAIR(each, eval(CAR(vals), env), NULL, -1);
    SET_CDR(index, each);
    index = each;
    vars = CDR(vars);
    vals = CDR(vals);
  }
  if (vals != NULL) {
    ERRORF(old_vars->position, 函数实际参数数目错误);
  }
  return first;
}
/**
   执行非原生函数
**/
atom *procedure(atom *body, atom *vars, atom *vals, atom *env) {
  atom *new_frame;
  MAKE_PAIR(new_frame, vars, vals, -1);
  atom *new_env;
  MAKE_PAIR(new_env, new_frame, env, -1);
  return eval_sequence(body, new_env);
}

/***
    apply
**/
atom *apply(atom *exp, atom *env) {
  atom *ret = is_primary_func_and_exe(exp, env);
  if (ret != NULL)
    return ret;
  else {
    //非原生函数
    if (IS(SYMBOL, CAR(exp))) {
      //非lambda
      atom *value = lookup_variable_value(CAR(exp), env);
      if (value == NULL) {
	ERRORF(exp->position, 函数不存在);
      } else {
	atom *after_eval_args = eval_list_return_list(DATA(value).FUNCTION.params, CDR(exp), env);
	//执行，返回结果
	return procedure(DATA(value).FUNCTION.body, DATA(value).FUNCTION.params, after_eval_args, env);
      }
    } else {
      //lambda
      if (!IS(PAIR, CAR(exp))) {
	//不是lambda类型，报错
	ERRORF(exp->position, 此处应该为函数类型);
      }
      if (!is_special(CAR(exp), "lambda")) {
	ERRORF(exp->position, 用法不正确);
      }
      atom *lambda = eval_lambda(CAR(exp), env);
      atom *after_eval_args = eval_list_return_list(DATA(lambda).FUNCTION.params, CDR(exp), env);
      return procedure(DATA(lambda).FUNCTION.body, DATA(lambda).FUNCTION.params, after_eval_args, env);
    }
  }
}
