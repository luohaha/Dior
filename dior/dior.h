#ifndef DIOR_H
#define DIOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/**
   基础的原子类型：
   布尔boolean
   数字number
   字符串string
   字符character
   序对pair
   符号symbol
   自定义函数
**/
typedef enum __atom_type atom_type;
typedef union __atom_data atom_data;
typedef struct __atom atom;
typedef struct __pair pair;

/**
   原子类型的枚举
**/
enum __atom_type {
  BOOLEAN = 1, NUMBER, STRING, CHARACTER, PAIR, PRIMITIVE_FUNC, SYMBOL, FUNCTION
};

/**
   原子类型的结构
**/
union __atom_data {
  struct {
    char value;
  } BOOLEAN;
  
  struct {
    long value;
  } NUMBER;
  
  struct {
    char *value;
  } STRING;

  struct {
    char value; 
  } CHARACTER;

  struct {
    atom *car;
    atom *cdr;
  } PAIR;

  struct {
    char *value;
  } SYMBOL;

  struct {
    atom *params;
    atom *body;
    atom *env; //函数的求值环境
  } FUNCTION;

};

struct __atom {
  atom_type type;
  atom_data data;
  int position;//atom出现在文件中的行数，用来debug
};

#define IS(t, x) (x->type == t)

#define DATA(x) x->data
//根据type类型，获取原子x的实际值
#define GET_VALUE(type, x) (DATA(x).type.value)
//获取pair的car
#define CAR(x) (DATA(x).PAIR.car)
//获取cdr
#define CDR(x) (DATA(x).PAIR.cdr)
#define CAAR(x) CAR(CAR(x))
#define CADR(x) CAR(CDR(x))
#define CADDR(x) CAR(CDR(CDR(x)))
#define CADDDR(x) CAR(CDR(CDR(CDR(x))))
#define CDDR(x) CDR(CDR(x))

#define SET_VALUE(type, x, v) (DATA(x).type).value = v
#define SET_CAR(x, v) DATA(x).PAIR.car = v
#define SET_CDR(x, v) DATA(x).PAIR.cdr = v

#define MAKE_ATOM(t, x, v, p)	    \
  x = (atom*) malloc(sizeof(atom)); \
  x->type = t; \
  x->position = p; \
  SET_VALUE(t, x, v)

#define COPY_ATOM(new, old) \
  new = (atom*) malloc(sizeof(atom)); \
  new->type = old->type; \
  new->position = old->position; \
  new->data = old->data
  
#define MAKE_PAIR(x, car, cdr, p)	    \
  x = (atom*) malloc(sizeof(atom)); \
  x->type = PAIR; \
  x->position = p; \
  SET_CAR(x, car); \
  SET_CDR(x, cdr)
  
#define MAKE_FUNC(x, p, b, e, po)    \
  x = (atom*) malloc(sizeof(atom)); \
  x->type = FUNCTION; \
  x->position = po;		  \
  DATA(x).FUNCTION.params = p;	  \
  DATA(x).FUNCTION.body = b;      \
  DATA(x).FUNCTION.env = e

#define ERRORF(line, err) \
  fprintf(stderr, "in line %d error : "#err"\n", line); \
  exit(-1)

#define MAX_STRING 10000 //词法分析数组的大小
#define MAX_WORD_SIZE 40 //每个词的大小

extern char *lex_list[MAX_STRING]; //词法分析结果
extern int lex_index;
extern int lex_list_row[MAX_STRING];

//extern atom *ast;
//lexer.c
void lexer(FILE *file);
//parser.c
atom *parser();
//env.c
atom *init_env();
atom *lookup_variable_value(atom *var, atom *env);
atom *extend_environment(atom *vars, atom *vals, atom *env);
void define_variable(atom *var, atom *val, atom *env);
int set_variable_value(atom *var, atom *val, atom *env);
//eval.c
atom *eval_sequence(atom *exp, atom *env);
atom *eval(atom *exp, atom *env);
atom *eval_lambda(atom *exp, atom *env);
void make_sure_usage(atom *exp, int num, const char* type);
//apply.c
atom *apply(atom *exp, atom *env);
int is_special(atom *exp, const char *type);
//primitive_func.c
atom *padd(atom *args, atom *env);
atom *pmul(atom *args, atom *env);
atom *psub(atom *args, atom *env);
atom *pdiv(atom *args, atom *env);
atom *pequal(atom *args, atom *env, const char *sym);
atom *list(atom *exp, atom *env);
atom *cons(atom *exp, atom *env);
atom *car(atom *exp, atom *env);
atom *cdr(atom *exp, atom *env);
atom *pprint(atom *exp, atom *env);
atom *pprintln(atom *exp, atom *env);
atom *quote(atom *exp, atom *env);
atom *require(atom *exp, atom *env);
atom *set_car(atom *exp, atom *env);
atom *set_cdr(atom *exp, atom *env);
atom *is_type(atom *exp, atom *env, atom_type type);
atom *append(atom *exp, atom *env);

//gc.c
//void free_atom(atom *exp);
//main.c
void print_atom(atom *exp);
FILE *file_test(char *filename);
void clean_lex();

#endif
