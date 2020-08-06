/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KBD_PORT		0x60
#define KBD_CTRL_PORT		0x64

#define KBD_ACK			0xFA
#define KBD_ECHO_RESPONSE	0xEE
#define KBD_ERROR_1		0x00
#define KBD_ERROR_2		0xFF
#define KBD_RESEND		0xFE
#define KBD_TEST_FAILED_1	0xFC
#define KBD_TEST_FAILED_2	0xFD
#define KBD_TEST_PASSED		0xAA

typedef enum keypress_state{
	KEYPRESS_MAKE,		// aka press...
	KEYPRESS_BREAK		// ..and release
	} keypress_state;

typedef struct key_action_t{
	uint8_t key;		// encoded row-col position on abstract keyboard
	keypress_state state;	// MAKE or BREAK
	} key_action_t;

#ifndef _KEYBOARD_C
extern uint8_t keyboard_buffer_start, keyboard_buffer_end;

void keyboard_init();
void keyboard_irq_read();
#else
#include "abstract_keyboard.h"

void keyboard_remove_command_queue();
void keyboard_send_command_queue();
#endif

#ifndef _KEYBOARDBUFFER_H
key_action_t keyboard_buffer[256];
uint8_t keyboard_buffer_start, keyboard_buffer_end;

void keyboard_buffer_add(key_action_t add);
key_action_t keyboard_buffer_consume(void);
#else
extern key_action_t keyboard_buffer[256];
#endif

#endif