// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/console/drivers/drivers.h>
#include <sys/console/drivers/serial_console/serial_console.h>
#include <sys/devicemgr/deviceapi/deviceapi_serial.h>
#include <dev/serial/serial.h>

void serial_console_init(){	
	return;
}

uint8_t serial_console_write(const char *c) {
	 serial_write_string(c);
}

void serial_console_register(){
	console_interfaces[CONSOLE_DRIVER_SERIAL].init = &serial_console_init;
	console_interfaces[CONSOLE_DRIVER_SERIAL].setpos =0;
	console_interfaces[CONSOLE_DRIVER_SERIAL].write = &serial_console_write;
	
	return;
}
