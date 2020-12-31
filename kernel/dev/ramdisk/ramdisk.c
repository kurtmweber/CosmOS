//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/ramdisk/ramdisk.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <types.h>
#include <debug/assert.h>
#include <devicemgr/deviceapi/deviceapi_ata.h>
#include <mm/mm.h>
#include <string/mem.h>

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
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    kprintf("Init %s of size %llu MB at %#hX (%s)\n",dev->description,(deviceData->size)/1024, deviceData->data, dev->name);
    memset(deviceData->data, 0, deviceData->size);
}

uint64_t ramdisk_calc_address(uint64_t data, uint32_t sector){
    return data+(sector*RAMDISK_SECTOR_SIZE);
}

void ramdisk_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");
    ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");
    ASSERT(size <= RAMDISK_SECTOR_SIZE, "data too large for sector");
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    ASSERT(sector < RAMDISK_TOTAL_SECTORS, "invalid sector");
    uint64_t block = ramdisk_calc_address((uint64_t)deviceData->data, sector);
    kprintf("block %#hX\n", block);
    memcpy(data, (uint8_t*)block, size);
}

void ramdisk_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");
    ASSERT_NOT_NULL(dev->deviceData, "dev->deviceData cannot be null");
    ASSERT(size <= RAMDISK_SECTOR_SIZE, "data too large for sector");
    struct ramdisk_devicedata* deviceData = (struct ramdisk_devicedata*) dev->deviceData;
    ASSERT(sector < RAMDISK_TOTAL_SECTORS, "invalid sector");
    uint64_t block = ramdisk_calc_address((uint64_t)deviceData->data, sector);
    kprintf("block %#hX\n", block);
    memcpy((uint8_t*)block, data, size);  
}

uint16_t ramdisk_sector_size(struct device* dev) {
    return RAMDISK_SECTOR_SIZE;
}
uint32_t ramdisk_total_sectors(struct device* dev){
    return RAMDISK_TOTAL_SECTORS;
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
    struct deviceapi_ata* api = (struct deviceapi_ata*) kmalloc(sizeof(struct deviceapi_ata));
    api->write = &ramdisk_write;
    api->read = &ramdisk_read;
    api->sector_size = &ramdisk_sector_size;
    api->total_sectors = &ramdisk_total_sectors;
    deviceinstance->api = api;
    /*
    * register
    */
    devicemgr_register_device(deviceinstance);
}
