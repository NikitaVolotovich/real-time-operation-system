#include <printf.h>
#include "gen_tasks.h"
#include "clock.h"

void gen_task_job_1(struct scheduler* s, void* f) {
    UNUSED(s);
    UNUSED(f);
    printf("%i: gen_task_job_1 was executed\n", clock_time());
}

struct task GEN_TASK_1 = {.period = 8,
			  .max_execution_time = 3,
			  .relative_deadline = 8,
			  .job = &gen_task_job_1,
			  .data = NULL};

void gen_task_job_2(struct scheduler* s, void* f) {
    UNUSED(s);
    UNUSED(f);
    printf("%i: gen_task_job_2 was executed\n", clock_time());
}

struct task GEN_TASK_2 = {.period = 14,
			  .max_execution_time = 4,
			  .relative_deadline = 12,
			  .job = &gen_task_job_2,
			  .data = NULL};

void gen_task_job_3(struct scheduler* s, void* f) {
    UNUSED(s);
    UNUSED(f);
    printf("%i: gen_task_job_3 was executed\n", clock_time());
}

struct task GEN_TASK_3 = {.period = 41,
			  .max_execution_time = 4,
			  .relative_deadline = 19,
			  .job = &gen_task_job_3,
			  .data = NULL};

