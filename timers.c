// timers.c
// Author: Maxwell Herron

#include "timers.h"

// Sets all the necessary values to enable timer zero with the desired match
void setup_timer_zero(int match) {
  // Setting the OCR to the input match value (derived from the equation)
  OCR0A = match;
  // Turning on CTC mode
  TCCR0B |= (1 << WGM02);
  // Setting CS02, CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
  // Enabling the timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

// Default setup for pwm mode on timer one
void setup_timer_one_fast_pwm() {
  // Turning on Fast PWM mmode
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  // Setting the top value
  OCR1A = 0xFFFF;
  // Setting the match value
  OCR1B = OCR1A / 2;
  // Setting the CS bits for 1024 prescaler
  TCCR1B |= (1 << CS02) | (1 << CS00);
  // Setting the COM bits
  TCCR1A |= (1 << COM1B1);
}

void setup_ms_timer(void) {
	// set timer control register part B to use timer 0
	// timer 0 is an 8-bit timer
	// Sets WGM0 to use CTC mode (hence the 01)
	TCCR0A |= (1<<WGM01);

	// set timer counter control register part B to use timer 0
	// clock select 0 or 1 bits (uses 64 bits)
	TCCR0B |= (1<<CS00) | (1<<CS01);

	// timer 0 channel A
	// (16,000,000 ticks / 1 sec) * (1 cnt / 64 ticks) * (1 int / 250 cnt) = 1000int/sec
	// interrupt fires once every 1ms and increments ms_ticks e/ time
	OCR0A = 250;

	// Enables the interrupt OCIE0A
	TIMSK0 |= (1<<OCIE0A);
}

// Takes an input value to set the new match
void set_pwm_value(uint32_t value) {
  OCR1A = 16000000 / (1024.0 * value);  // Calculating the new top value
  OCR1B = 16000000 / (1024.0 * value) / 2;  // Calculating the new match value
}

ISR(TIMER0_COMPA_vect) {
  ++ms_ticks;
}
