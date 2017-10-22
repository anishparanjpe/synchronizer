/* Toy synchronizer: Sample template
 * Your synchronizer should implement the three functions listed below.
 */

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

#include "traverse.h"

extern int sem_cnt;
extern int ctr;
extern int res_cnt;

sem_t* sems[20];
int ctrs[20];

pthread_mutex_t lock;

void P(result_t* result)
{
  sem_wait(sems[result->sem1]);
}

void PP(result_t* result)
{
  sem_wait(sems[result->sem1]);
  ctrs[result->ctr]++;
  if (ctrs[result->ctr] == 1)
    sem_wait(sems[result->sem2]);
  sem_post(sems[result->sem1]);
}

void V(result_t* result)
{
  sem_post(sems[result->sem1]);
}

void VV(result_t* result)
{
  sem_wait(sems[result->sem1]);
  ctrs[result->ctr]--;
  if (ctrs[result->ctr] == 0)
    sem_post(sems[result->sem2]);
  sem_post(sems[result->sem1]);
}

void ENTER_OPERATION(const char *op_name)
{
  result_t* result = find_result(op_name, 1);
  if (!strcmp(result->op, " P")) P(result);
  else PP(result);
}

void EXIT_OPERATION(const char *op_name)
{
  result_t* result = find_result(op_name, 0);
  if (!strcmp(result->op, " V")) V(result);
  else VV(result);
}

void INIT_SYNCHRONIZER(const char *path_exp)
{
  int i;
  char buf[10];

  printf("Initializing Synchronizer with path_exp %s\n", path_exp);

  prepare();
  traverse(path_exp);

  for (i = 0; i < sem_info_cnt; i++) {
    int idx = sem_infos[i]->cnt;
    int value = sem_infos[i]->value;
    sprintf(buf, "S%d", idx);
    sems[idx] = sem_open(buf, O_CREAT | O_EXCL, 0644, value);
    sem_unlink(buf);
  }

  for (i = 1; i <= ctr; i++) {
    ctrs[i] = 0;
  }

  pthread_mutex_init(&lock, NULL);
}
