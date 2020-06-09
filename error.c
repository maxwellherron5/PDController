#include "error.h"

// Start with blinking leds!

void initialize_error() {
  configure_led(&_yellow);
  configure_led(&_green);
}

void initialize_debug() {
  initialize_error();
}

int error() {
  if (_yellow.configured) {
    flash_led(&_yellow);
    flash_led(&_yellow);
    flash_led(&_yellow);
  }
  return ERROR;
}

void debug() {
  if (_green.configured) {
    flash_led(&_green);
  }
}
