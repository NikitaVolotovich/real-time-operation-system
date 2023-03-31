#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "custom_tasks.h"
#include "gen_tasks.h"
#include "scheduler.h"
#include "sporadic_tasks.h"

#include "task.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf(
            "Scheduler expects as an argument a name of taskset to schedule: "
            "custom, generated, sporadic ");
        return 1;
    }

    clock_init();
    struct scheduler sched;
    char* name = argv[1];

    if (strcmp(name, "custom") == 0) {
        struct task* generic_set[4] = {&LED_TASK, &UART_TASK, &FIB_TASK,&CUSTOM_TASK};
        schedule(&sched, generic_set, 4, 0);
    } else if (strcmp(name, "generated") == 0) {
        struct task* gen_set[3] = {&GEN_TASK_1, &GEN_TASK_2, &GEN_TASK_3};
        schedule(&sched, gen_set, 3, 0);
    } else if (strcmp(name, "sporadic") == 0) {
        struct task* sporadic_set[2] = {&SPOR_TASK_1, &SPOR_TASK_2};
        schedule(&sched, sporadic_set, 2, 1);
    } else {
        printf("%s is unknown taskset", name);
    }

    return 0;
}
