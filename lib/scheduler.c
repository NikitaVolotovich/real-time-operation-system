#include <stdio.h>
#include "scheduler.h"
#include "clock.h"

#define FALSE 0
#define TRUE 1
#define SERVER 2 // I used define for better readability, and it cannot be solution for another count of tasks

#define LOGS
#define WARNINGS
#define DESCRIPTIONS

struct sporadic_queue_manager sporadic_manager;

task_unit server = {
        .is_sporadic = 1,
        .period_start = 0,
        .period = 1000,
        .max_execution_time = 20,
        .relative_deadline = 2000,
        .job = &sporadic_server_job,
        .data = NULL
};

void sporadic_server_job(struct scheduler* sched, void* data) {
    UNUSED(data);
    struct sporadic_event event = get_event_from_queue();
    if(event.job == NULL){
        #ifdef LOGS
            printf("Log: %d: Sporadic job is NULL.\n", clock_time());
        #endif
        return;
    }
    event.job(sched, event.data);
}

void schedule(struct scheduler* sched, struct task** tasks, int count,
              int has_sporadic) {
    (void)(has_sporadic);

    if(count < 1)
        return;

    sched->tasks = tasks;
    sched->task_count = count;

    task_unit jobs[TASKS_MAX_COUNT * 16];
    sched->task = jobs;

    for(uint16_t i = 0; i < count; i++) {
        task_unit task = {
                .is_sporadic = 0,
                .period_start = 0,
                .period = tasks[i]->period,
                .max_execution_time = tasks[i]->max_execution_time,
                .relative_deadline = tasks[i]->relative_deadline,
                .job = tasks[i]->job,
                .data = tasks[i]->data
        };
        sched->task[i] = task;
    }

    if(has_sporadic) {
        sporadic_manager.isOn = TRUE;
        sporadic_manager.isReady = TRUE;
        sporadic_manager.counter = 1;
        sched->task[SERVER] = server;
        count += 1;
    }

    sched->task_count = count;

    while(TRUE) {
        if(sporadic_manager.isOn){
            if(clock_time() - sched->task[SERVER].period * sporadic_manager.counter < 0) // When uint64_t will start new cycle
                sporadic_manager.counter = 1;
        }

        int task_for_execution_id = get_next_task_by_edf(sched->task, sched->task_count);
        task_unit task_to_exec = sched->task[task_for_execution_id];

        sched->current_task_id = task_for_execution_id;

        if (task_to_exec.period_start > clock_time())
            clock_delay_ms(task_to_exec.period_start - clock_time());

        if(is_overrun(task_to_exec.max_execution_time, task_to_exec.period_start, task_to_exec.period,
                      task_to_exec.relative_deadline))
            return;

        sched->task[task_for_execution_id].period_start = task_to_exec.period_start + task_to_exec.period;

        uint32_t time = clock_time();

        task_to_exec.job(sched, task_to_exec.data);
        uint32_t execution_time = clock_time() - time;

        if(execution_time > task_to_exec.max_execution_time){
            #ifdef WARNINGS
                printf("Warning: Task[%i] overrun %d ms\n", task_for_execution_id, execution_time - task_to_exec.max_execution_time);
            #endif
        }
    }

}

int scheduler_on_sporadic(struct scheduler* sched, struct sporadic_event e) {
    if(clock_time() >= sched->task[SERVER].period * sporadic_manager.counter){
        // I prefer some timers interrupt on MCU, with period = server period and change flag status with it.
        // But in this virtual reality, I set flag by this way.
        #ifdef DESCRIPTIONS
            printf("Description: %d >= %d * %d\n", clock_time(),sched->task[SERVER].period ,sporadic_manager.counter);
        #endif
        sporadic_manager.counter += (clock_time() / sched->task[SERVER].period) - sporadic_manager.counter;
        // ^ Mostly it's only increase by 1, but for the case when during the period it's no sporadic event's at all
        sporadic_manager.isReady = TRUE;
    }

    if(!sporadic_manager.isReady){
        #ifdef WARNINGS
            printf("Warning: %d: At this period sporadic job was already scheduled. Event skipped.\n", clock_time());
        #endif
        return 0;
    }

    add_event_to_sporadic_queue(e);
    sporadic_manager.isReady = FALSE;

    #ifdef LOGS
        printf("Log: %d: Job was added to the sporadic queue.\n", clock_time());
    #endif
    return 1;
}

void add_event_to_sporadic_queue(struct sporadic_event e){
    if(sporadic_manager.events[0].job == NULL){
        sporadic_manager.events[0] = e;
        #ifdef LOGS
            printf("Log: %d: job at slot [0] added to queue.\n", clock_time());
        #endif
        return;
    }
    if(sporadic_manager.events[1].job == NULL){
        sporadic_manager.events[1] = e;
        #ifdef LOGS
            printf("Log: %d: job at slot [1] added to queue.\n", clock_time());
        #endif
        return;
    }
    printf("Error: Both frames in sporadic queue are busy. Unable to add sporadic job.\n");
}

void shift_sporadic_queue(){
    sporadic_manager.events[0] = sporadic_manager.events[1];
    sporadic_manager.events[1].job = NULL;
    sporadic_manager.events[1].data = NULL;
    #ifdef LOGS
        printf("Log: %d: sporadic events were shifted.\n", clock_time());
    #endif
}

struct sporadic_event get_event_from_queue(){
    struct sporadic_event temp_event = sporadic_manager.events[0];
    shift_sporadic_queue();
    return temp_event;
}

int get_next_task_by_edf(task_unit* tasks, int task_count) {
    uint32_t time = clock_time();
    int16_t next_task = -1;
    int16_t released_tasks[TASKS_MAX_COUNT * 16];
    int16_t released_count = 0;

    for(uint16_t i = 0; i < task_count; i++) {
        if(tasks[i].period_start <= time) {
            released_tasks[released_count] = i;
            released_count++;
        }
    }

    if(released_count > 0) {
        next_task = released_tasks[0];
        for (uint16_t i = 1; i < released_count; i++)
            if(tasks[released_tasks[i]].period_start + tasks[released_tasks[i]].relative_deadline <
               tasks[next_task].period_start + tasks[next_task].relative_deadline)
                next_task = released_tasks[i];
        return next_task;
    }

    next_task = 0;
    for(uint16_t i = 0; i < task_count; i++)
        if(tasks[i].period_start < tasks[next_task].period_start)
            next_task = i;

    return next_task;
}

int calc_required_time(struct scheduler* sched, uint32_t absolute_deadline, uint32_t start_time) {
    int32_t total_time = absolute_deadline - start_time;

    task_unit tasks[TASKS_MAX_COUNT * 16];
    uint16_t task_count = sched->task_count;

    for (int16_t i = 0; i < task_count; i++) {
        tasks[i] = sched->task[i];
    }

    while(TRUE) {
        uint16_t task_to_exec_id = get_next_task_by_edf(tasks, task_count);

        if (tasks[task_to_exec_id].period_start > absolute_deadline)
            break;

        if(tasks[task_to_exec_id].period_start + tasks[task_to_exec_id].max_execution_time <= absolute_deadline)
            total_time -= tasks[task_to_exec_id].max_execution_time;
        else
            total_time -= absolute_deadline - tasks[task_to_exec_id].period_start;

        if (tasks[task_to_exec_id].is_sporadic)
            task_count--;
        else
            tasks[task_to_exec_id].period_start = tasks[task_to_exec_id].period_start + tasks[task_to_exec_id].period;
    }
    return total_time;
}

uint8_t is_overrun(uint32_t max_execution_time, uint32_t period_start, uint32_t period, uint32_t relative_deadline){
    if (clock_time() + max_execution_time > period_start + relative_deadline) {
        uint32_t current_time = clock_time();
        uint32_t overrun = (current_time + max_execution_time) - (period_start + relative_deadline);
        printf("Error: overrun by %d\n", overrun);
#ifdef DESCRIPTIONS
        printf("Description, time: %d: period start: %d; period: %d; deadline: %d. \n", current_time, period_start,
               period, period_start + relative_deadline);
#endif
        return 1;
    }
    return 0;
}

void schedule_single_task(struct scheduler *sched, struct task *task_ptr) {
    while(TRUE){
        uint32_t end_time = clock_time() + task_ptr->period;
        task_ptr -> job(sched, task_ptr -> data);
        clock_delay_ms(end_time - clock_time());
    }
}