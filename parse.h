#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdlib.h>

#include "stack.h"

typedef struct node_s {
  int type;
  char name[20];
  struct node_s* lt;
  struct node_s* rt;
} node_t;

const char* lookahead;

node_t* parse(const char*);
void free_ast(node_t*);

#endif
