
// These files handle error checking and debuggin.
// If communication is not functional,
// then errors/debug will come in the form of blinking on-board leds in a
// particular pattern.
// If communication is working, then print statements.
// Possibly blink and print.

#ifndef ERROR_H_
#define ERROR_H_

#include "common.h"
#include "leds.h"

#define ERROR -1

void initialize_error();
void initialize_debug();
int error();
void debug();

#endif
