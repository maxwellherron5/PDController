/*
 * pd.h
 *
 * Created: 04/06/2020
 * Author : Maxwell Herron
 */
#ifndef PD_H_
#define PD_H_

#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern volatile uint32_t ms_ticks;
extern volatile int16_t global_counts_m2;

int32_t degrees_to_encoder(int32_t degrees);
void handle_ui();
void pd_task();
void execute_command(char* input, int *value);
void execute_setpoints();
void is_arrived();
void execute_setpoint(int position);
void read_setpoints(int *trajectories);
int32_t encoder_to_degrees(int32_t encoder_count);
void potentiometer_task();
void logging_task();

#endif
