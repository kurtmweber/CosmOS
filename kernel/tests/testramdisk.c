//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testramdisk.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/string/string.h>
#include <sys/debug/assert.h>
#include <sys/string/mem.h>

uint8_t* testdata = "We were very tired, we were very merry, \
We had gone back and forth all night on the ferry. \
We hailed, Good morrow, mother! to a shawl-covered head, \
And bought a morning paper, which neither of us read; \
And she wept, God bless you! for the apples and pears, \
And we gave her all our money but our subway fares.";

void test_ramdisk() {
	// get virtual block device
	uint8_t devicename[] ={"rd0"};

	struct device* ata = devicemgr_findDevice(devicename);
	if (0!=ata){
		struct deviceapi_block* ata_api = (struct deviceapi_block*) ata->api;

		uint16_t data[256];
	    memset((uint8_t*)data, 0, 255*sizeof(uint16_t));

		(*ata_api->write)(ata, 7, (uint16_t*) testdata, strlen(testdata));

		uint16_t readdata[255];
	    memset((uint8_t*)readdata, 0, 255*sizeof(uint16_t));

		(*ata_api->read)(ata, 7, readdata, 512);


		debug_show_memblock((uint8_t*)readdata, 32);
		ASSERT(((uint8_t*)readdata)[0]=='W', "oops");
		ASSERT(strlen(((uint8_t*)readdata))==strlen(testdata), "oops!");
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}



