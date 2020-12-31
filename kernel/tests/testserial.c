//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testserial.h>
#include <devicemgr/deviceapi/deviceapi_serial.h>
#include <console/console.h>
#include <debug/debug.h>

/*
* write a message via the uniform serial api
*/
void serialMessage(const uint8_t* message) {
   	// get serial0
	struct device* serial0 = devicemgr_findDevice("serial0");
	if (0!=serial0){
		struct deviceapi_serial* serial_api = (struct deviceapi_serial*) serial0->api;
		serial_write_function write_func = serial_api->write;
		(*write_func)(serial0, message);
	} else {
		kprintf("Unable to find serial0\n");
	}
}
