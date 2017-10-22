#ifndef _STACK_H_
#define _STACK_H_

typedef struct info_s {
  char op[3];
  int ctr, sem1, sem2;
} info_t;

typedef struct info_stack_s {
  info_t* contents[10];
  int top;
} info_stack_t;

info_t* new_info(char*, int, int, int);
void push(info_stack_t*, info_t*);
void pop(info_stack_t*);
info_t* top(info_stack_t*);

#endif
