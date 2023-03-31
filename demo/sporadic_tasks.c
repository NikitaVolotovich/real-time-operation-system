#include "sporadic_tasks.h"

#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "scheduler.h"

void sporadic_fun(struct scheduler* s, void* raw_ptr) {
  UNUSED(s);
  UNUSED(raw_ptr);
  printf("%i: something random happend\n", clock_time());
}

// Sporadic Task 1
// ----------------------------------------------------------------------------
// May create a sporadic generic_task

void spor_job_1(struct scheduler* sched, void* raw_ptr) {
  UNUSED(raw_ptr);
  if (rand() % 42 == 0) {
    struct sporadic_event e = {.relative_deadline = 2000,
			       .max_execution_time = 3,
			       .job = &sporadic_fun,
			       .data = NULL};
    scheduler_on_sporadic(sched, e);
  }
}

struct task SPOR_TASK_1 = {.period = 16,
			   .max_execution_time = 11,
			   .relative_deadline = 16,
			   .job = &spor_job_1,
			   .data = NULL};

// Sporadic Task 2
// ----------------------------------------------------------------------------
// May create a sporadic generic_task

void spor_job_2(struct scheduler* sched, void* raw_ptr) {
  UNUSED(raw_ptr);
  if (rand() % 42 == 0) {
    struct sporadic_event e = {.relative_deadline = 6666,
			       .max_execution_time = 3,
			       .job = &sporadic_fun,
			       .data = NULL};
    scheduler_on_sporadic(sched, e);
  }
}

struct task SPOR_TASK_2 = {.period = 74,
			   .max_execution_time = 5,
			   .relative_deadline = 32,
			   .job = &spor_job_2,
			   .data = NULL};
