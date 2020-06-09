#ifndef TIMERS_H
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#define WGM00 0
#define WGM01 1

// Granting access to the global var ms_ticks
extern volatile uint32_t ms_ticks;

// Sets all the necessary values to enable timer zero with the desired match
void setup_timer_zero(int match);

// Sets all the necessary values to enable timer zero with the desired match
void setup_timer_one(int match);

// Sets up timer one in pwm mode
void setup_timer_one_fast_pwm();

void set_pwm_value(uint32_t value);

void setup_ms_timer(void);

#endif
