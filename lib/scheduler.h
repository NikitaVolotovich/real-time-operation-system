#include "task.h"

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef struct generic_task {
    int is_sporadic;
    uint32_t period_start;
    uint32_t period;
    uint32_t max_execution_time;
    uint32_t relative_deadline;
    job_ptr job;
    void* data;
} task_unit;

// struct containing data for the scheduler, you can modify this up to your will
struct scheduler {
    struct task** tasks;
    int task_count;
    task_unit* task;
    int current_task_id;
    uint16_t spor_job_counter;
};

struct sporadic_queue_manager {
    uint8_t isOn;
    uint8_t isReady;
    uint32_t counter;
    struct sporadic_event events[2];
};
// = {0, {{NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL}}};

// This is the main scheduling function, you receive a pointer to array of
// pointers to tasks structures. Count of tasks in the array. A bool variable
// `has_sporadic` that is 1 in case the scheduler should expect a sporadic generic_task
// and 0 when it should not expect sporadic generic_task.
//
// Remember to use the `SCHEDULER` global variable and
void schedule(struct scheduler* sched, struct task** tasks, int count,
              int has_sporadic);

// Function that notifies scheduler of new sporadic event that should be
// scheduled by it. It returns 1 in case of success and 0 in case the event was
// declined.
int scheduler_on_sporadic(struct scheduler* sched, struct sporadic_event e);

void schedule_single_task(struct scheduler *sched, struct task *task_ptr);

int is_sporadic_active(struct scheduler* sched);
int calc_required_time(struct scheduler* sched, uint32_t absolute_deadline, uint32_t start_time);
int get_next_task_by_edf(task_unit* tasks, int task_count);
uint8_t is_overrun(uint32_t max_execution_time, uint32_t period_start, uint32_t period, uint32_t relative_deadline);
void update_time(uint32_t *time);
void add_event_to_sporadic_queue(struct sporadic_event e);
void shift_sporadic_queue();
void sporadic_server_job(struct scheduler* sched, void* data);
struct sporadic_event get_event_from_queue();
#endif

