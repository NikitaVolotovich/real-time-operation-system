cmake_minimum_required(VERSION 3.10)

project(scheduler)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Wall -Wextra -O0 -g)

add_library(scheduler 
    lib/scheduler.c
    lib/clock.cpp
    )
target_include_directories(scheduler PUBLIC lib/)

add_executable(demo
    demo/main.c
    demo/custom_tasks.c
    demo/gen_tasks.c
    demo/sporadic_tasks.c
    )
target_include_directories(demo PRIVATE demo/)
target_link_libraries(demo PRIVATE scheduler)
