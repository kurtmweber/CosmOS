//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <tests/testata.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/console/console.h>
#include <sys/debug/debug.h>
#include <sys/string/string.h>
#include <sys/debug/assert.h>
#include <sys/string/mem.h>
#include <sys/i386/mm/mm.h>

void test_ata() {
	// get virtual block device
	uint8_t devicename[] ={"disk0"};

	struct device* ata = devicemgr_findDevice(devicename);
	if (0!=ata){
		struct deviceapi_block* ata_api = (struct deviceapi_block*) ata->api;

		uint16_t sector_size = (*ata_api->sector_size)(ata);
		kprintf("Sector size %llu\n",sector_size);

		uint32_t total_size = (*ata_api->total_size)(ata);
		kprintf("Total size %llu\n",total_size);

		uint32_t buffersize = sizeof(uint8_t)*sector_size;

		uint8_t* data = kmalloc(buffersize);
		memset(data, 0, buffersize);

		(*ata_api->read)(ata, 0, data, 1); // 1 sector
		debug_show_memblock(data, 64);
	} else {
		kprintf("Unable to find %s\n",devicename);
	}
}

