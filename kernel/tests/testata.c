//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testata.h>
#include <devicemgr/deviceapi/deviceapi_ata.h>
#include <console/console.h>
#include <debug/debug.h>
#include <string/string.h>
#include <debug/assert.h>

uint8_t* testdata = "We were very tired, we were very merry, \
We had gone back and forth all night on the ferry. \
We hailed, Good morrow, mother! to a shawl-covered head, \
And bought a morning paper, which neither of us read; \
And she wept, God bless you! for the apples and pears, \
And we gave her all our money but our subway fares.";

void test_ata() {
	// get virtual block device
	uint8_t devicename[] ={"rd0"};

	struct device* ata = devicemgr_findDevice(devicename);
	if (0!=ata){
		struct deviceapi_ata* ata_api = (struct deviceapi_ata*) ata->api;

		uint8_t data[256];
		(*ata_api->write)(ata, 7, testdata, strlen(testdata));

		uint8_t readdata[512];
		(*ata_api->read)(ata, 7, readdata, 512);


		debug_show_memblock(readdata, 32);
		ASSERT(readdata[0]=='W', "oops");
		ASSERT(strlen(readdata)==strlen(testdata), "oops!");
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}

void test_ata2() {
	// get virtual block device
	uint8_t devicename[] ={"ata1"};

	struct device* ata = devicemgr_findDevice(devicename);
	if (0!=ata){
		struct deviceapi_ata* ata_api = (struct deviceapi_ata*) ata->api;

		uint8_t data[256];
		(*ata_api->read)(ata, 0, data, 255);
		debug_show_memblock(data, 32);
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}

