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
   原始函数primitive_func
   符号symbol
   自定义函数
**/
typedef enum __atom_type atom_type;
typedef union __atom_data atom_data;
typedef struct __atom atom;

/**
   原子类型的枚举
**/
enum __atom_type {
  BOOLEAN = 1, NUMBER, STRING, CHARACTER, PAIR, PRIMITIVE_FUNC, SYMBOL
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

#define SET_VALUE(type, x, v) (DATA(x).type).value = v
#define SET_CAR(x, v) DATA(x).PAIR.car = v
#define SET_CDR(x, v) DATA(x).PAIR.cdr = v

#define MAKE_ATOM(t, x, v, p)	    \
  x = (atom*) malloc(sizeof(atom)); \
  x->type = t; \
  x->position = p; \
  SET_VALUE(t, x, v)
#define MAKE_PAIR(x, car, cdr, p)	    \
  x = (atom*) malloc(sizeof(atom)); \
  x->type = PAIR; \
  x->position = p; \
  SET_CAR(x, car); \
  SET_CDR(x, cdr)
  

#define ERRORF(line, err) \
  fprintf(stderr, "in line %d error : "#err"\n", line); \
  exit(-1)

#define MAX_STRING 10000 //词法分析数组的大小
#define MAX_WORD_SIZE 40 //每个词的大小

extern char *lex_list[MAX_STRING]; //词法分析结果
extern int lex_index;
extern int lex_list_row[MAX_STRING];

extern atom *ast;
void lexer(FILE *file);
void parser();
#endif