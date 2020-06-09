// leds.c
//
// Author: Amy Larson

#include "leds.h"

LED_struct _yellow = {
  &DDRC, &PORTC, &PINC,
  YELLOW, 0, 0  };  // not inverted, not configured.
LED_struct _red = {
  &DDRB, &PORTB, &PINB,
  RED, 1, 0 };    // inverted, not configured.
LED_struct _green = {
  &DDRD, &PORTD, &PIND,
    GREEN, 1, 0 };  // inverted, not configured.

/* configure the data direction for the specified on-board led.
 */
void configure_led(LED_struct * color) {
  SET_BIT(*color->ddr, color->pin);
  color->configured = 1;
}

void led_on(LED_struct * color) {
  // define this. (look at flash_led for inspiration)
    if (!color->inverted) {
      SET_BIT(*color->port, color->pin);
  } else {
      CLEAR_BIT(*color->port, color->pin);
  }
}

void led_off(LED_struct * color) {
    if (!color->inverted) {
      CLEAR_BIT(*color->port, color->pin);
  } else {
      SET_BIT(*color->port, color->pin);
  }
}

void led_toggle(LED_struct * color) {
  TOGGLE_BIT(*color->port, color->pin);
}

/* Flash the designated on-board led on, delay, off, delay. */
void flash_led(LED_struct * color) {
  if (!color->inverted) {
    SET_BIT(*color->port, color->pin);
  } else {
    CLEAR_BIT(*color->port, color->pin);
  }
  _delay_ms(200);
  TOGGLE_BIT(*color->port, color->pin);
  _delay_ms(200);
}

/* Flash all the initialized leds for a sanity check light show */
void light_show() {
  int i;
  for (i = 0; i < 2; i++) {
    if (_yellow.configured) flash_led(&_yellow);
    if (_red.configured) flash_led(&_red);
    if (_green.configured) flash_led(&_green);
  }
}
