### For the scheduler implementation `EDF` algorithm was used.

Problem: sometimes on Windows 11 tasks are overrun (scheduler.c line:58), 
but on macOS Monterey it works normal. 

### Schedule on sporadic event
![SCHEDULE ON SPORADIC](https://github.com/NikitaVolotovich/real-time-operation-system/blob/main/2022-06-17%2010.19.08.jpg "")
- Sporadic manager has 2 frames for a jobs.
- The main rule is: first job in the period should be always scheduled. Other's are cancelled.
![SPORADIC QUEUE VISUALISATION](https://github.com/NikitaVolotovich/real-time-operation-system/blob/main/2022-06-17%2010.32.18.jpg "")


### Project logging
- In the file `scheduler.c` it's possible to define/undefine 'LOGS', 'WARNINGS', 'DESCRIPTIONS' 
to see/hide information about sporadic events.
## First attempt:

### The Nitpicks:
 - Readability of: "data->i = data->i += 15" is very poor and I think it is a
   bug, I think you put superfluous "=" in there

### The Good:
 - Nicely readable code, except the nitpick bit
 - Overrun detection is not needed but welcome
 - Custom task
 - Scheduling metric is good

### The Problems:
 - The assignment says you must use some kind of a server, I do not see server
   being used
 - Inability to schedule more than one sporadic task, it is required if sporadic
   task is accepted at 4999ms of the first period, it might not be finished
   before another task is to be accepted at 1ms of the second period

### Extra notes:
 - You can use the has_sporadic to just add a server into the task set if it
   is true
 - I did not analyze the scheduler_on_sporadic too deeply, as server is not
   used, it is not correct anyway
 - If you have any more questions, feel free to reach out to us via email

## Second attempt:

- How about making a function with parameters (scheduler* sched, void* data)
which then checks the sporadic queue and executes a job from it, you could
add it as a job to the sporadic server, eliminating special logic from the
scheduler itself.

- You define sporadic_queue_manager in the header file, resulting in multiple
definitions, this actually causes our tests to not compile, move it to
scheduler.c

- Good: Your have the exact sporadic event capacity, now set the deadline
so that you are always guaranteed to run the jobs in time even if two release
at once and both are accepted. If you manage to do that, you will fulfill the
4999ms and 5001ms sporadic event requirement.

