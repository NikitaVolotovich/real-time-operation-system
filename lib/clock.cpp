#include "clock.h"

#include <chrono>
#include <thread>

// What is the point of this file and API?
//
// Well, this is necessary on embedded  devices, where you have to manually
// prepare/setup your clock for the system. It is usually implemented with
// these functions:
//
//  - clock_init() is required to setup the hardware peripheral of the
//  embedded device for counting the time. (Simple timer)
//  - clock_time() returns time based on how the timer counted it
//  - clock_delay_ms() is just handy function for sleeping that used the
//  facillity provided by the timer
//
// In case of desktop version it is not necessary, we prefer to use the same API.
// It has certain benefits for testing from our side, we also figured that it is
// little bit easier to get proper clock in C++ than C in multiplatform way.
//
// Note that the clock is NOT reliable and may cause you problems with testing
// with which you have to deal yourself.

std::chrono::system_clock::time_point start;

extern "C" {

void clock_init() { start = std::chrono::system_clock::now(); }

uint32_t clock_time() {
    auto t_now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t_now - start)
        .count();
}

void clock_delay_ms(uint32_t n) {
    std::chrono::milliseconds t{n};
    std::this_thread::sleep_for(t);
}
}
