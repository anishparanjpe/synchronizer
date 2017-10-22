#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

extern const char* lookahead;

node_t* new_node(int type, char* name, node_t* lt, node_t* rt)
{
  node_t* node = malloc(sizeof(node_t));
  node->type = type;
  strcpy(node->name, name);
  node->lt = lt;
  node->rt = rt;
  return node;
}

void free_ast(node_t *node)
{
  if (node->lt) free_ast(node->lt);
  if (node->rt) free_ast(node->rt);
  free(node);
}

void match(const char* str)
{
  int i;

  for (i = 0; i < strlen(str); i++)
    if (*lookahead == str[i])
      lookahead++;
    else {
      printf("Parsing error: expected %c found %c while matching %s\n", str[i], *lookahead, str);
      exit(1);
    }
}

void parse_string(char *buf)
{
  int i = 0;
  while (isalpha(*lookahead)) { *(buf + i++) = *lookahead; lookahead++; }
  *(buf + i) = '\0';
}

node_t* parse_choice();

node_t* parse_base()
{
  char buf[20];
  node_t* node;

  if (*lookahead == '(') {
    match("(");
    node = parse_choice();
    match(")");
    return node;
  }
  parse_string(buf);
  return new_node('B', buf, NULL, NULL);
}

node_t* parse_concurrent()
{
  node_t* lt;

  if (*lookahead == '{') {
    match("{");
    lt = parse_base();
    match("}");
    return new_node('*', "concurrent", lt, NULL);
  }
  return parse_base();
}

node_t* parse_sequence()
{
  node_t* lt = parse_concurrent();

  if (*lookahead == ';') {
    match(";");
    return new_node(';', "sequence", lt, parse_sequence());
  }
  return lt;
}

node_t* parse_choice()
{
  node_t* lt = parse_sequence();

  if (*lookahead == '+') {
    match("+");
    return new_node('+', "choice", lt, parse_choice());
  }
  return lt;
}

node_t* parse_path_expr()
{
  match("path ");
  node_t* lt = parse_choice();
  match(" end");

  if (*lookahead == ' ') {
    match(" ");
    return new_node('P', "path", lt, parse_path_expr());
  }
  return new_node('P', "path", lt, NULL);
}

node_t* parse(const char* path_exp)
{
  lookahead = path_exp;
  return parse_path_expr();
}
