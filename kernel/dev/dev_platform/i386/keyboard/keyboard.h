/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <dev/dev_platform/i386/keyboard/abstract_keyboard.h>

void keyboard_devicemgr_register_devices();

void keyboard_remove_command_queue();
void keyboard_send_command_queue();

#endif
