#include "custom_tasks.h"

#include <stdio.h>
#include <stdlib.h>

#include "clock.h"


// LED task
// ----------------------------------------------------------------------------
// blinks a led

struct led_task_data {
    uint8_t i;
};

void led_task_job(struct scheduler* s, void* raw_ptr) {
    UNUSED(s);
    struct led_task_data* data = raw_ptr;
    printf("Status of green led: %i\r\n", data->i % 2);
    data->i = (data->i + 1) % 2;
}

struct led_task_data LED_TASK_DATA = {.i = 0};
struct task LED_TASK = {.period = 250,
                        .max_execution_time = 1,
                        .relative_deadline = 50,
                        .job = &led_task_job,
                        .data = (void*)&LED_TASK_DATA};
// UART task
// ----------------------------------------------------------------------------
// writes data to UART - standard communication line with computer
// (not really relevant on desktop version)

void uart_task_job(struct scheduler* s, void* raw_ptr) {
    UNUSED(s);
    UNUSED(raw_ptr);
    printf("UART TASK: %i\r\n", clock_time());
}

struct task UART_TASK = {.period = 251,
                         .max_execution_time = 40,
                         .relative_deadline = 251,
                         .job = &uart_task_job,
                         .data = NULL};

// FIB task
// ----------------------------------------------------------------------------
// Naive implemetnation of fibonnaci sequence. What is the distribution of
// execution time of this function?

uint32_t fib_rec(uint32_t v) {
    if (v == 1 || v == 0) {
        return 1;
    }
    return fib_rec(v - 1) + fib_rec(v - 2);
}

void fib_task_job(struct scheduler* s, void* raw_ptr) {
    UNUSED(s);
    UNUSED(raw_ptr);
    uint32_t time = clock_time();
    // average length of computation for fib[24] number is 34 ms (on embedded
    // hardware)
    uint32_t i = time % 25;
    uint32_t val = fib_rec(i);
    printf("FIB TASK: fib[%i] is: %i \r\n", i, val);
}

struct task FIB_TASK = {.period = 1499,
                        .max_execution_time = 40,
                        .relative_deadline = 1499,
                        .job = &fib_task_job,
                        .data = NULL};

// Custom task
// ----------------------------------------------------------------------------
// Implement this one by yourself.

void custom_task_job(struct scheduler* s, void* raw_ptr) {
    struct led_task_data *data = raw_ptr;
    printf("RS485 buffer: %i\n", data->i);
    data->i += 15;
}

struct led_task_data RS485_data = {.i = 0};

struct task CUSTOM_TASK = {.period = 100,
                           .max_execution_time = 10,
                           .relative_deadline = 100,
                           .job = custom_task_job,
                           .data = (void *) &RS485_data};
