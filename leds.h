// Author: Amy Larson

#ifndef LEDS_H_
#define LEDS_H_

#include "common.h"
#include <util/delay.h>

typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  volatile uint8_t * portin;
  uint8_t pin;
  uint8_t inverted;
  uint8_t configured;
} LED_struct;

// These function as enumerated types (using pin numbers as values)
#define GREEN 5
#define YELLOW 7
#define RED 0

// Global structures for each on-board LED
extern LED_struct _yellow;
extern LED_struct _red;
extern LED_struct _green;

/* configure the on-board led for use.
 */
void configure_led(LED_struct * color);

/* Flash (on/off once) indicated on-board led */
void flash_led(LED_struct * color);

/* Flash all the configured leds for a sanity check light show */
void light_show();

/* turn on specified on board LED. */
void led_on(LED_struct * color);

/* turn off specified on board LED.*/
void led_off(LED_struct * color);

/* toggle the led between on and off */
void led_toggle(LED_struct * color);

#endif
