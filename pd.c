/*
* pd.c
*
* Author: Maxwell Herron
* Contains all necessary functions to allow for PD control.
*/

#include "analog.h"
#include "pd.h"
#include "motor.h"
#include "lufa.h"

#define SETPOINT_LEN 10

// Used to differentiate if user is selecting UI option or pressing enter.
char taking_input = false;
// Keeps track of which UI task was chosen when enter is pressed.
char ui_task;
int32_t stopAt;
int32_t lastTime;
int32_t lastError;
int8_t is_logging = false;
int32_t velocity;
int torque;
int32_t setpoints[SETPOINT_LEN] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
uint8_t setpoint_idx = 0;
char arrived = true; // flag for determing if arrived at setpoint
int kD = 0;
int kP = 0;
char logging = false;

// Determines if current angle is within an acceptance state
void is_arrived() {
  if (velocity == 0 && abs(stopAt - global_counts_m2) < 250) {
    arrived = true;
  }
}

// Sets the new reference position after converting input
// degrees to encoder units, then determines direction.
void execute_setpoint(int position) {
  stopAt = degrees_to_encoder(position);
  global_counts_m2 = 0;
  if (stopAt < 0) { motorBackward(); }
  else { motorForward(); }
}

// Iterates through the array of waypoints, and delegates the execution
// to the auxilliary function execute_setpoint
void execute_setpoints() {
  if (taking_input == true) {
    int i;
    for (i = setpoint_idx; i < SETPOINT_LEN; i++) {
      if (arrived == true) {
        int curr = (int) setpoints[i];
        execute_setpoint(curr);
        arrived = false;
        setpoint_idx++;
      }
    }
    if (setpoint_idx == SETPOINT_LEN) {
      setpoint_idx = 0;
      taking_input = false;
    }
  }
}

// Leaving this here to copy and paste to test trajectories
// r 1000 222 -432 360 -321 1000 -3222 873 -1223 403

// Reads list of input waypoints and assigns them into array
void read_setpoints(int *trajectories) {
  int i;
  for (i = 0; i < SETPOINT_LEN; i++) {
    if (trajectories[i] <= 32767 && trajectories[i] >= (-32768)) {
      setpoints[i] = trajectories[i];
    }
  }
}

// Converts input degrees to encounder counts.
int32_t degrees_to_encoder(int32_t degrees) {
  int32_t encoder_setpoint = degrees * (2249.0 / 360.0);
  return encoder_setpoint;
}

// Converts the input encoder count to degrees.
int32_t encoder_to_degrees(int32_t encoder_count) {
  int32_t degrees = encoder_count / (2249.0 / 360.0);
  return degrees;
}

// Determines what the input command is, and executes accordingly.
void execute_command(char* input, int *value) {
    if (!strcmp(input, "R") || !strcmp(input,"r")) {
      read_setpoints(value);
    }
    else if (!strcmp(input, "Z") || !strcmp(input,"z")) {
      global_counts_m2 = 0;
    }
    else if (!strcmp(input, "V") || !strcmp(input,"v")) {
      printf("Printing current values: \r\n");
      printf("Kd: %d\r\n", (int) adc_read(10));
      printf("Kp: %d\r\n", (int) adc_read(8));
      printf("Torque: %d\r\n", torque);
      printf("Vm: %ld\r\n", velocity);
      printf("Pr: %ld\r\n", stopAt);
      printf("Pm: %d\r\n", global_counts_m2);
    }
    else if (!strcmp(input, "S") || !strcmp(input,"s")) {
        logging = logging ? false : true;
    }
    else if (!strcmp(input, "T") || !strcmp(input,"t")) {
      taking_input = true;
    }
}

// Top level UI task, simply makes check for it commands are ready.
// If so, it will execute them accordingly.
void handle_ui() {
  if (user_command_ready()) {
    handle_command();
  }
}

// Runs all calculations of PD equation to determine necessary Torque
void pd_task() {
  int error = stopAt - global_counts_m2; // calculate the current error (P value)
  int delta_error = lastError - error; // calculate delta error
  int delta_Time = lastTime - ms_ticks; // calculate change in time
  velocity = delta_error / delta_Time;
  torque = ((kP / 1020.0) * error) + ((kD / 1020.0) * velocity);
  if (torque < 0) { // if T is negative switch motor to move backward
    motorBackward();
    torque = abs(torque);
  } else {
    motorForward();
  }
  OCR1B = torque;
  lastError = error;
  lastTime = ms_ticks;
  is_arrived();
}

// Updates the gain values by reading from each potentiometer
void potentiometer_task() {
  kD = adc_read(10);
  kP = adc_read(8);
}

// If logging is set to true, prints vals. Use this with the -L screen command.
void logging_task() {
  if (logging) {
    printf("kD: %d kP: %d Torque: %d Vm: %ld Pr: %ld Pm: %d\r\n", kD, kP, torque, velocity, stopAt, global_counts_m2);
  }
}
