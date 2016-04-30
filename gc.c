#include "dior.h"

/**
   Yixin Luo
   2016/5/1
   垃圾回收部分
**/
/**
   回收atom
**/
void free_atom(atom *exp) {
  if (IS(BOOLEAN, exp)) {
    free(exp);
  } else if (IS(NUMBER, exp)) {
    free(exp);
  } else if (IS(STRING, exp)) {
    free(GET_VALUE(STRING, exp));
    free(exp);
  } else if (IS(CHARACTER, exp)) {
    free(exp);
  } else if (IS(PAIR, exp)) {
    free(CAR(exp));
    free(CDR(exp));
    free(exp);
  } else if (IS(SYMBOL, exp)) {
    free(GET_VALUE(SYMBOL, exp));
    free(exp);
  } else if (IS(FUNCTION, exp)) {
    free(DATA(exp).FUNCTION.params);
    free(DATA(exp).FUNCTION.body);
    free(exp);
  }
}
