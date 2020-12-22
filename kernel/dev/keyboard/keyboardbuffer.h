/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARDBUFFER_H
#define _KEYBOARDBUFFER_H

#include <types.h>
#include <devicemgr/deviceapi/deviceapi_keyboard.h>

extern key_action_t keyboard_buffer[256];
extern uint8_t keyboard_buffer_start, keyboard_buffer_end;

void keyboard_buffer_add(key_action_t add);
key_action_t keyboard_buffer_consume(void);
void keyboard_buffer_init();

#endif