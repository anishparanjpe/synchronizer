#include <stdlib.h>
#include <string.h>

#include "stack.h"

info_t* new_info(char* op, int ctr, int sem1, int sem2)
{
  info_t* info = malloc(sizeof(info_t));
  strcpy(info->op, op);
  info->ctr = ctr;
  info->sem1 = sem1;
  info->sem2 = sem2;
  return info;
}

void push(info_stack_t* stack, info_t* info) { stack->contents[++(stack->top)] = info; }
void pop(info_stack_t* stack) { stack->top--; }
info_t* top(info_stack_t* stack) { return stack->contents[stack->top]; }
