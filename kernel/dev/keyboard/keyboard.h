/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <dev/keyboard/abstract_keyboard.h>

typedef enum keypress_state{
	KEYPRESS_MAKE,		// aka press...
	KEYPRESS_BREAK		// ..and release
	} keypress_state;

typedef struct key_action_t{
	uint8_t key;		// encoded row-col position on abstract keyboard
	keypress_state state;	// MAKE or BREAK
	} key_action_t;

void keyboard_register_devices();
void keyboard_irq_read();
void keyboard_remove_command_queue();
void keyboard_send_command_queue();

extern key_action_t keyboard_buffer[256];
extern uint8_t keyboard_buffer_start, keyboard_buffer_end;

void keyboard_buffer_add(key_action_t add);
key_action_t keyboard_buffer_consume(void);

#endif
