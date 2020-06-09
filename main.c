// main.c
// Author: Maxwell Herron

#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include "common.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "leds.h"
#include "timers.h"
#include "pd.h"
#include "analog.h"
#include "scheduler.h"


/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/

// Acts as the reference point that you want your encoder value to be when
// the motor is prompted to move.
int16_t reference_position = 0;
// Keeps track of the direction of the motor motion.
uint8_t direction = 1;
// millisecond counters
volatile uint32_t ms_ticks = 0;
volatile uint32_t start_time;

void init_tasks() {
	spawn_task(100, handle_ui);
	spawn_task(50, pd_task);
	spawn_task(400, execute_setpoints);
	spawn_task(25, potentiometer_task);
	spawn_task(250, logging_task);
}

void initialize_system(void) {
	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	setup_ms_timer();
	light_show();
  setupMotor2();
  setupEncoder();
  motorForward();
	initialize_pot();
	setup_ms_timer();
	SetupHardware();
	init_tasks();
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();
  sei();

	// Give user time to enter screen
	_delay_ms(5000);

	printf("Welcome to my wonderful PD control system. Here are the commands:\r\n");
	printf("R/r followed by a list of up to ten waypoints will set the motor waypoints.\r\n");
	printf("Z/z will zero the encoder position.\r\n");
	printf("V/v will print out all current values (of importance).\r\n");
	printf("S/s will enable logging.\r\n");
	printf("T/t will set the motor to execute all input waypoints\r\n");
	printf("Enjoy!\r\n");
  while(1) {
		USB_Mainloop_Handler();

		scheduler();
		server();

  } /* end while(1) loop */
} /* end main() */
