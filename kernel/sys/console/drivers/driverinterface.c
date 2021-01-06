/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <sys/console/console.h>
#include <sys/console/drivers/drivers.h>
#include <sys/console/drivers/vga_console/vga_console.h>
#include <sys/console/drivers/serial_console/serial_console.h>

void console_driver_interface_init(){
	uint8_t i;

	serial_console_register();
	vga_console_register();
	
	for (i = 0; i < CONSOLE_DRIVER_LAST; i++){
		console_interfaces[i].init();
	}
}

uint8_t console_select_driver(console_driver d){
	if (d >= CONSOLE_DRIVER_LAST){
		return 0;
	} else {
		console_active_driver = d;
		return 1;
	}
}

uint8_t console_setpos(uint8_t x, uint8_t y){
	return console_interfaces[console_active_driver].setpos(x, y);
}

uint8_t console_write(const char *c){
	return console_interfaces[console_active_driver].write(c);
}
