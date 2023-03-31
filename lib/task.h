#include "stddef.h"
#include "stdint.h"

#define TASKS_MAX_COUNT 16

#ifndef TASK_H_
#define TASK_H_

// All time units are in milliseconds - ms;
struct scheduler;

// Type of pointer to generic_task of a task.
typedef void (*job_ptr)(struct scheduler*, void*);

// Structure containg all information about one task.
struct task {
    uint32_t period;              // ms
    uint32_t max_execution_time;  // ms
    uint32_t relative_deadline;   // ms
    job_ptr job;                  // generic_task to be executed
    void* data;  // data passed to the generic_task function each time it is called
};

// Structure containing about sporadic event.
struct sporadic_event {
    uint32_t relative_deadline;   // ms
    uint32_t max_execution_time;  // ms
    job_ptr job;                  // generic_task to be executed
    void* data;                   // data for the generic_task
};

#define UNUSED(x) (void)(x)

#endif
