#include <stdint.h>

#ifndef CLOCK_H_
#define CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

// Setups the clock to calculate milliseconds counter. Starts counting from
// the moment the clock_init is called.
void clock_init();

// Delays execution until n milliseconds has passed.
void clock_delay_ms(uint32_t n);

// Returns actual count of milliseconds from call to clock_init()
uint32_t clock_time();

#ifdef __cplusplus
}
#endif

#endif
