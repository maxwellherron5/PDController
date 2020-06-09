#include "lufa.h"
#include "pd.h"

#define COMMAND_BUFF_LEN 80

char menu[50] = "MENU not defined";
char command[COMMAND_BUFF_LEN+1];
int my_index = 0;
char buffer_ready = false;

void default_process(char* buffer) {
  char option[10];
  int value[10];
  sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d",option,
    &value[0], &value[1], &value[2], &value[3], &value[4] ,&value[5], &value[6],
    &value[7], &value[8], &value[9]);
  execute_command(option, value);
}

void(*process_command)(char*) = default_process;

void initialize_lufa() {
    SetupHardware(); //This setups the USB hardware and stdio
    //process_command = default_process;
}

uint8_t user_command_ready() {

  if (buffer_ready) {
    printf("Command Buffer Ready. Not processed. Processing now.\n\r");
    USB_Mainloop_Handler();
    handle_command();
  }

  USB_Mainloop_Handler();
  char c = fgetc(stdin);
  while (c != EOF) {
    if (my_index >= COMMAND_BUFF_LEN) {
      return true;
    }
    // if its backspace, reset index
    if (c == '\b') {
      my_index--;
      if (my_index < 0) my_index=0;
      printf("\b \b");
    }
    // if newline, go back 1 to write over
    /* how can it be newline??
    else if (c == '\n') {
      index--;
      if (index < 0) index=0;
    }
    */
    // if return, we have complete input
    else if (c == '\r') {
      command[my_index] = '\0';
      buffer_ready = true;
      // echo back the return
      printf("\r\n");
      USB_Mainloop_Handler();
      return true;
    }

    // otherwise, we got a char - echo it back
    printf("%c", c);
    USB_Mainloop_Handler();
    command[my_index] = c;
    my_index++;

    USB_Mainloop_Handler();
    c = fgetc(stdin);
  }
  return false;
}

void handle_command( ) {
  process_command(command);
  USB_Mainloop_Handler();
  my_index = 0;
  buffer_ready = false;
}
