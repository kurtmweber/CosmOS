//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/fs/cfs/cfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/fs/block_util.h>
#include <sys/console/console.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/debug.h>
#include <sys/string/mem.h>

void cfs_list_dir(struct device* dev, struct list* lst) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");    
    ASSERT_NOT_NULL(lst, "lst cannot be null");

    uint16_t sector_size = block_get_sector_size(dev);
    kprintf("sector size: %llu\n", sector_size);

    uint32_t total_size = block_get_total_size(dev);
    kprintf("total size: %llu\n", total_size);

    uint8_t* buffer = kmalloc(sector_size);
	memset(buffer, 0, sector_size);

    block_read(dev, 0, buffer,1);
}
