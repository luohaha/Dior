#include "dior.h"
/**
   Yixin Luo
   2016/4/26
   这个部分负责实现词法分析器，将从文件中输入的字符串，转换为字符串数组

**/

char *lex_list[MAX_STRING]; //词法分析结果
int lex_index = 0;
int lex_list_row[MAX_STRING]; // 对应每个词，它出现的行数
int line = 1;
/**
   偷窥
**/
char peek(FILE *input) {
  int c;
  c = getc(input);
  ungetc(c, input);
  return c;
}
/**
   跳过注释，注释以;开头
**/
void pass_note(FILE *input, char *c) {
  while (((*c = getc(input)) != EOF) && (*c != '\n'));
}
/**
   读入一个词
**/
void read_word(FILE *input, char c) {
  int i = 0;
  lex_list[lex_index] = malloc(MAX_WORD_SIZE);
  lex_list[lex_index][i++] = c;
  while ((c = getc(input)) != EOF && c != ')' && c != '(' \
	 && c != ';' && !isspace(c)) {
    lex_list[lex_index][i++] = c;
  }
  lex_list[lex_index][i] = '\0';
  lex_list_row[lex_index++] = line;
  ungetc(c, input);
}
/**
   从文件流中读取输入，生成字符串数组
**/
void lexer(FILE *input) {
  char c;
  int sign = 1;
  int num; // 存储读入的数字
  lex_list[lex_index] = malloc(2);
  lex_list[lex_index++] = "(";
  while ((c = getc(input)) != EOF) {
    if (c == ';')
      pass_note(input, &c);
    if (c == EOF)
      break;
    if (c == '\n')
      line++;
    if (isspace(c))
      continue;
    if (c == '(') {
      lex_list[lex_index] = malloc(2);
      lex_list_row[lex_index] = line;
      lex_list[lex_index++] = "(";
      continue;
    }
    if (c == ')') {
      lex_list[lex_index] = malloc(2);
      lex_list_row[lex_index] = line;
      lex_list[lex_index++] = ")";
      continue;
    }
    read_word(input, c);
  } //while
  lex_list[lex_index] = malloc(2);
  lex_list[lex_index++] = ")";
}
