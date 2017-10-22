#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "stack.h"
#include "traverse.h"

result_t* new_result(char* name, char* op, int ctr, int sem1, int sem2)
{
  result_t *result = malloc(sizeof(result_t));
  strcpy(result->name, name);
  strcpy(result->op, op);
  result->ctr = ctr;
  result->sem1 = sem1;
  result->sem2 = sem2;
  return result;
}

sem_info_t* new_sem_info(int cnt, int value)
{
  sem_info_t *sem_info = malloc(sizeof(result_t));
  sem_info->cnt = cnt;
  sem_info->value = value;
  return sem_info;
}

void prepare()
{
  entry_stack.top = -1;
  exit_stack.top = -1;
  sem_info_cnt = 0;
  sem_cnt = 0;
  ctr = 0;
  res_cnt = 0;
}

result_t* find_result(const char* name, int entry)
{
  int i;

  for (i = 0; i < res_cnt; i++) {
    result_t* result = results[i];
    if (!strcmp(result->name, name)) {
      if (entry) {
        if (!strcmp(result->op, "PP") || !strcmp(result->op, " P")) { return result; }
      } else {
        if (!strcmp(result->op, "VV") || !strcmp(result->op, " V")) { return result; }
      }
    }
  }
  return NULL;
}

void pop_entry_exit_stacks()
{
  pop(&entry_stack);
  pop(&exit_stack);
}

void traverse_internal(node_t* root)
{
  int cnt;
  info_t *entry_top, *exit_top;

  switch (root->type) {
  case 'P':
    cnt = ++sem_cnt;
    sem_infos[sem_info_cnt++] = new_sem_info(cnt, 1);
    push(&entry_stack, new_info(" P", -1, cnt, -1));
    push(&exit_stack, new_info(" V", -1, cnt, -1));

    traverse_internal(root->lt);

    if (root->rt) traverse_internal(root->rt);
    pop_entry_exit_stacks();
    break;
  case '+':
    push(&entry_stack, top(&entry_stack));
    push(&exit_stack, top(&exit_stack));

    traverse_internal(root->lt);
    traverse_internal(root->rt);
    pop_entry_exit_stacks();
    break;
  case ';':
    cnt = ++sem_cnt;
    sem_infos[sem_info_cnt++] = new_sem_info(cnt, 0);

    push(&entry_stack, top(&entry_stack));
    push(&exit_stack, new_info(" V", -1, cnt, -1));
    traverse_internal(root->lt);
    pop_entry_exit_stacks();

    push(&entry_stack, new_info(" P", -1, cnt, -1));
    push(&exit_stack, top(&exit_stack));
    traverse_internal(root->rt);
    pop_entry_exit_stacks();
    break;
  case '*':
    cnt = ++sem_cnt;
    sem_infos[sem_info_cnt++] = new_sem_info(cnt, 1);

    entry_top = top(&entry_stack);
    exit_top = top(&exit_stack);
    push(&entry_stack, new_info("PP", ++ctr, cnt, entry_top->sem1));
    push(&exit_stack, new_info("VV", ctr, cnt, exit_top->sem1));

    traverse_internal(root->lt);
    pop_entry_exit_stacks();
    break;
  case 'B':
    entry_top = top(&entry_stack);
    exit_top = top(&exit_stack);
    if (!strcmp(entry_top->op, "PP")) {
      results[res_cnt++] =
        new_result(root->name, entry_top->op,
                   entry_top->ctr, entry_top->sem1, entry_top->sem2);
      results[res_cnt++] =
        new_result(root->name, exit_top->op,
                   exit_top->ctr, exit_top->sem1, exit_top->sem2);
    } else {
      results[res_cnt++] =
        new_result(root->name, entry_top->op,
                   -1, entry_top->sem1, -1);
      results[res_cnt++] =
        new_result(root->name, exit_top->op,
                   -1, exit_top->sem1, -1);
    }
    printf("%s_%d_%d_%d %s %s_%d_%d_%d\n",
        entry_top->op, entry_top->ctr, entry_top->sem1, entry_top->sem2, root->name,
        exit_top->op,  exit_top->ctr,  exit_top->sem1,  exit_top->sem2);
    break;
  default: printf("ERROR\n");
  }
}

void traverse(const char* path_expr) {
  node_t *root = parse(path_expr);
  traverse_internal(root);
  free_ast(root);
}
