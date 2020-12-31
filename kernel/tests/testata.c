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

void test_ata() {
	// get the ata
	struct device* ata = devicemgr_findDevice("ata0");
	if (0!=ata){
		struct deviceapi_ata* ata_api = (struct deviceapi_ata*) ata->api;

		uint8_t data[256];
		(*ata_api->read)(ata, 0, data, 255);
		debug_show_memblock(data, 32);
	} else {
		kprintf("Unable to find ata0\n");
	}
}

