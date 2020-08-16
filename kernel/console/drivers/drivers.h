/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _CONSOLE_DRIVERS_H
#define _CONSOLE_DRIVERS_H

typedef enum console_driver{
	CONSOLE_DRIVER_VGA = 0,
	CONSOLE_DRIVER_LAST
} console_driver;

typedef struct console_driver_interface_t{
	void (*init)();
	uint8_t (*setpos)(uint8_t x, uint8_t y);
	uint8_t (*write)(const char *s);
} console_driver_interface_t;

#ifndef _CONSOLE_DRIVERS_C
extern console_driver_interface_t console_interfaces[CONSOLE_DRIVER_LAST];

void console_driver_interface_init();
uint8_t console_setpos(uint8_t x, uint8_t y);
uint8_t console_write(const char *c);
#else
console_driver console_active_driver;
console_driver_interface_t console_interfaces[CONSOLE_DRIVER_LAST];
#endif

#endif