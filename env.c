#include "dior.h"
/**
   Yixin Luo
   2016/4/29
   和环境变量有关的操作
**/
/**
   初始化环境变量
**/
atom *init_env() {
  atom *frame; // 保存变量和值的一一对应的帧
  MAKE_PAIR(frame, NULL, NULL, -1);
  atom *node; //连接的节点
  MAKE_PAIR(node, frame, NULL, -1);
  return node;
}

/**
   在环境中查找var，但是返回pair
**/
atom *lookup_variable_value_pair(atom *var, atom *env) {
  if (!IS(SYMBOL, var)) {
    ERRORF(var->position, 类型不正确);
  }
  atom *tmp_env = env;
  atom *f; //每一帧frame
  while (tmp_env != NULL) {
    //在每一个frame中寻找
    f = CAR(tmp_env);
    atom *each_var = CAR(f);
    atom *each_value = CDR(f);
    while (each_var != NULL && each_value != NULL) {
      int cmp = strcmp(GET_VALUE(SYMBOL, CAR(each_var)), GET_VALUE(SYMBOL, var));
      if (cmp == 0)
	return each_value;
      else {
	each_var = CDR(each_var);
	each_value = CDR(each_value);
      }
    }
    //下一frame
    tmp_env = CDR(tmp_env);
  }
  //找不到
  return NULL;
}


/**
   在环境中查找var
**/
atom *lookup_variable_value(atom *var, atom *env) {
  atom *res = lookup_variable_value_pair(var, env);
  if (res == NULL)
    return NULL;
  else {
    return CAR(res);
  }
}

/**
   创建一个新的帧frame,插入新环境，并返回
**/
atom *extend_environment(atom *vars, atom *vals, atom *env) {
  if (vars == NULL) //当函数没有参数时，直接返回
    return env;
  atom *frame;
  MAKE_PAIR(frame, vars, vals, -1);
  atom *newNode;
  MAKE_PAIR(newNode, frame, env, -1);
  return newNode;
}

/**
   在环境的第一个框架里加入一个新的约束
**/
void define_variable(atom *var, atom *val, atom *env) {
  atom *first_frame = CAR(env);
  atom *new_car;
  //加入新的var
  MAKE_PAIR(new_car, var, CAR(first_frame), -1);
  SET_CAR(first_frame, new_car);
  atom *new_cdr;
  //加入新的val
  MAKE_PAIR(new_cdr, val, CDR(first_frame), -1);
  SET_CDR(first_frame, new_cdr);
  return;
}

/**
   修改var在环境env中的值val
**/
int set_variable_value(atom *var, atom *val, atom *env) {
  atom *res = lookup_variable_value_pair(var, env);
  if (res == NULL)
    return -1;
  else {
    free(CAR(res)); //回收原先的数据
    SET_CAR(res, val);
    return 0;
  }
}


