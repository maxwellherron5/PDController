#ifndef ANALOG_H_
#define ANALOG_H_

#include "common.h"

extern IO_struct potPower;
extern IO_struct potSignal;

// initialize adc
void initialize_adc();

// initialize potentiometer
void initialize_pot();

// take a reading from the specified channel
uint16_t adc_read(uint8_t channel);

#endif
