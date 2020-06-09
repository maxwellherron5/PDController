#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "common.h"

#define MAX_TASK_COUNT 10

typedef struct Task {
    uint8_t ready;
    uint16_t ms_period;
    uint16_t missed_deadlines;
    uint64_t next_release;
    void (*task_fn)(void);
} TASK_struct;

TASK_struct tasks[MAX_TASK_COUNT];
//extern uint8_t number_of_tasks;

void initialize_scheduler();

void spawn_task(uint16_t period, void(*function)(void) );

void scheduler();

void server();

#endif
