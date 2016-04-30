#include "dior.h"
/**
   Yixin Luo
   2016/4/28
   执行部分
**/

/**
   判断是否是自求值表达式
**/
int is_self_evaluating(atom *exp) {
  if (IS(BOOLEAN, exp))
    return 1;
  else if (IS(NUMBER, exp))
    return 1;
  else if (IS(STRING, exp))
    return 1;
  else if (IS(CHARACTER, exp))
    return 1;
  else
    return 0;
}

/**
   判断是否是特殊命令 set! define if lambda begin cond
**/
int is_special(atom *exp, const char *type) {
  if (IS(PAIR, exp)) {
    //首先要在()中
    atom *first = CAR(exp);
    //if (!IS(SYMBOL, first)) {
      //()的开头必须是特殊字符
    //  ERRORF(first->position, 用法不对);
    //}
    int cmp = strcmp(GET_VALUE(SYMBOL, first), type);
    if (cmp == 0)
      return 1;
    else
      return 0;
  } else
    return 0;
}

/**
   判断是否是变量
**/
int is_variable(atom *exp) {
  if (IS(SYMBOL, exp))
    return 1;
  else
    return 0;
}

/**
   执行赋值
**/
atom *eval_assignment(atom *exp, atom *env) {
  atom *val = eval(CADDR(exp), env);
  atom *var = CADR(exp);
  if (!IS(SYMBOL, var)) {
    ERRORF(var->position, 被赋值变量格式不正确);
  }
  int res = set_variable_value(var, val, env);
  if (res == -1) {
    ERRORF(exp->position, 被赋值的变量不存在);
  }
  return NULL;
}

/**
   执行定义操作
**/
atom *eval_definition(atom *exp, atom *env) {
  atom *var = CADR(exp);
  atom *val = eval(CADDR(exp), env);
  if (!IS(SYMBOL, var)) {
    ERRORF(var->position, 被赋值变量格式不正确);
  }
  define_variable(var, val, env);
  return NULL;
}

/**
   执行if
**/
atom *eval_if(atom *exp, atom *env) {
  atom *pre = eval(CADR(exp), env);
  if (!IS(BOOLEAN, pre)) {
    ERRORF(exp->position, if语句需要boolean类型);
  }
  if (GET_VALUE(BOOLEAN, pre) == 1) {
    //true
    return eval(CADDR(exp), env);
  } else {
    //false
    return eval(CADDDR(exp), env);
  }
}

/**
   执行lambda
**/
atom *eval_lambda(atom *exp, atom *env) {
  atom *params = CADR(exp);
  atom *body = CDDR(exp);
  atom *new_lambda;
  MAKE_FUNC(new_lambda, params, body, exp->position);
  return new_lambda;
}

/**
   执行begin
**/
atom *eval_begin(atom *exp, atom *env) {
  return eval_sequence(CDR(exp), env);
}

/**
   执行cond
**/
atom *eval_cond(atom *exp, atom *env) {
  atom *body = CDR(exp);
  atom *now;
  while (body != NULL) {
    now = CAR(body);
    atom *test = CAR(now);
    if (IS(SYMBOL, test)) {
      //可能是else
      int cmp = strcmp(GET_VALUE(SYMBOL, test), "else");
      if (cmp != 0) {
	//不是else，报错
	ERRORF(now->position, 此处应该为ELSE);
      } else {
	return eval_sequence(CDR(now), env);
      }
    } else {
      atom *test_ret = eval(test, env);
      if (!IS(BOOLEAN, test_ret)) {
	//不是boolean
	ERRORF(test_ret->position, COND必须基于BOOLEAN的比较);
      } else {
	if (GET_VALUE(BOOLEAN, test_ret) == 1) {
	  //true
	  return eval_sequence(CDR(now), env);
	} else {
	  //false
	  body = CDR(body);
	}
      }
    }
  }
  return NULL;
}

/**
   执行函数
**/
atom *eval_function(atom *exp, atom *env) {
  return apply(exp, env);
}

/**
   执行语句，exp是要执行的语句，env是环境变量
**/
atom *eval(atom *exp, atom *env) {
  if (is_self_evaluating(exp))
    return exp;
  else if (is_variable(exp))
    return lookup_variable_value(exp, env);
  else if (is_special(exp, "set!"))
    return eval_assignment(exp, env);
  else if (is_special(exp, "define"))
    return eval_definition(exp, env);
  else if (is_special(exp, "if"))
    return eval_if(exp, env);
  else if (is_special(exp, "lambda"))
    return eval_lambda(exp, env);
  else if (is_special(exp, "begin"))
    return eval_begin(exp, env);
  else if (is_special(exp, "cond"))
    return eval_cond(exp, env);
  else if (IS(PAIR, exp))
    return eval_function(exp, env);
  else {
    ERRORF(exp->position, 不符合用法);
  }
}

/**
   按顺序执行链表中的所有操作
**/
atom *eval_sequence(atom *exp, atom *env) {
  atom *list = exp;
  atom *now;
  atom *ret;
  while (list != NULL) {
    now = CAR(list);
    ret = eval(now, env);
    list = CDR(list);
  }
  return ret;
}
