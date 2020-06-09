
#ifndef LUFA_H_
#define LUFA_H_

#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#error VirtualSerial not defined!
#endif


#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#error **** VirtualSerial not defined, USB IO will not work  ****
#endif

#include "common.h"

#include <string.h>
#include <stdio.h>

extern char menu[50];

extern void(*process_command)(char*);

uint8_t user_command_ready();
void handle_command( );
void initialize_lufa();

#endif
