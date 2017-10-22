#ifndef _TRAVERSE_H_
#define _TRAVERSE_H_

#include "parse.h"
#include "stack.h"

typedef struct result_s {
  char name[20];
  char op[3];
  int ctr;
  int sem1;
  int sem2;
} result_t;

typedef struct sem_info_s {
  int cnt;
  int value;
} sem_info_t;

result_t* results[20];
sem_info_t* sem_infos[20];

info_stack_t entry_stack;
info_stack_t exit_stack;
int sem_info_cnt, sem_cnt, ctr, res_cnt;

void prepare(void);
result_t* find_result(const char* name, int entry);
void traverse(const char* path_exp);

#endif
