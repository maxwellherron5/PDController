#include "scheduler.h"
#include "timers.h"
#include "error.h"

void scheduler();
uint8_t number_of_tasks = 0;

// add task to the list of scheduled tasks
void spawn_task(uint16_t period, void(*function)(void) ) {
  if (number_of_tasks >= MAX_TASK_COUNT) {
    error();
    return;
  }
  tasks[number_of_tasks].ms_period = period;
  tasks[number_of_tasks].next_release = ms_ticks+period;
  tasks[number_of_tasks].task_fn = function;
  tasks[number_of_tasks].ready = 0;
  tasks[number_of_tasks].missed_deadlines = 0;
  ++number_of_tasks;
} // end spawn_task

// Update time. Check if any task is ready to run.
void scheduler() {
  ms_ticks++;
  int i;
  for(i = 0; i < number_of_tasks; i++) {
      if(ms_ticks >= tasks[i].next_release) {
          tasks[i].ready = 1;
          tasks[i].next_release = ms_ticks + tasks[i].ms_period;
      }
  }
} // end scheduler

void server() {
  int i;
  for(i = 0; i < number_of_tasks; i++) {
      if(tasks[i].ready) {
        tasks[i].task_fn();
        tasks[i].ready = 0;
      }
  }
} // end server
