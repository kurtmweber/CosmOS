//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/ramdisk/ramdisk.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <types.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/i386/mm/mm.h>
#include <sys/string/mem.h>

#define RAMDISK_SECTOR_SIZE 512
#define RAMDISK_TOTAL_SECTORS 1000

/*
* ramdisk instance specific data
*/
struct ramdisk_devicedata {
    uint8_t* data;
    uint64_t size;
} __attribute__((packed));

/*
* perform device instance specific init here
*/
void ramdisk_init(struct device* dev){
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    kprintf("Init %s of size %llu MB at %#hX (%s)\n",dev->description,(deviceData->size)/1024, deviceData->data, dev->name);
    memset(deviceData->data, 0, deviceData->size);
}

uint64_t ramdisk_calc_address(uint64_t data, uint32_t sector){
    return data+(sector*RAMDISK_SECTOR_SIZE);
}

void ramdisk_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT(count <= RAMDISK_SECTOR_SIZE);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    ASSERT(sector < RAMDISK_TOTAL_SECTORS);
    uint64_t block = ramdisk_calc_address((uint64_t)deviceData->data, sector);
    kprintf("block %#hX\n", block);
    memcpy((uint8_t*)data, (uint8_t*)block, count*sizeof(uint8_t));
}

void ramdisk_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT(count <= RAMDISK_SECTOR_SIZE);
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    ASSERT(sector < RAMDISK_TOTAL_SECTORS);
    uint64_t block = ramdisk_calc_address((uint64_t)deviceData->data, sector);
    kprintf("block %#hX\n", block);
    memcpy((uint8_t*)block, (uint8_t*)data, count*sizeof(uint8_t));  
}

uint16_t ramdisk_sector_size(struct device* dev) {
    return RAMDISK_SECTOR_SIZE;
}
uint32_t ramdisk_total_size(struct device* dev){
    return RAMDISK_TOTAL_SECTORS*RAMDISK_SECTOR_SIZE;
}

/**
*  register
*/
void ramdisk_devicemgr_register_devices() {
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &ramdisk_init;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAMDISK;
    devicemgr_set_device_description(deviceinstance, "RAM disk");
	/*
	* device data
	*/
	struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) kmalloc(sizeof(struct ramdisk_devicedata));
    deviceData->size= (uint64_t) RAMDISK_SECTOR_SIZE*RAMDISK_TOTAL_SECTORS;
    deviceData->data = kmalloc(deviceData->size);
	deviceinstance->deviceData = deviceData;
    /*
    * the device api
    */
    struct deviceapi_block* api = (struct deviceapi_block*) kmalloc(sizeof(struct deviceapi_block));
    api->write = &ramdisk_write;
    api->read = &ramdisk_read;
    api->sector_size = &ramdisk_sector_size;
    api->total_size = &ramdisk_total_size;
    deviceinstance->api = api;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}
