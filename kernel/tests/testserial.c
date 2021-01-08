//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testserial.h>
#include <sys/deviceapi/deviceapi_serial.h>
#include <sys/console/console.h>
#include <sys/debug/debug.h>

/*
* write a message via the uniform serial api
*/
void serialMessage(const uint8_t* message) {
   	// get serial0
	struct device* serial0 = devicemgr_find_device("serial0");
	if (0!=serial0){
		struct deviceapi_serial* serial_api = (struct deviceapi_serial*) serial0->api;
		serial_write_function write_func = serial_api->write;
		(*write_func)(serial0, message);
	} else {
		kprintf("Unable to find serial0\n");
	}
}

void test_serial() {
	/*
	* exercise the uniform serial API
	*/
	serialMessage("This message brought to you by the uniform serial API, the letters R and S and the Digits 2, 3 and 2\n");
}
