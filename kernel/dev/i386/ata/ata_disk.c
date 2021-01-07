/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/ata/ata_disk.h>
#include <dev/i386/ata/ata_util.h>
#include <dev/i386/ata/ata.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/i386/mm/mm.h>
#include <sys/debug/assert.h>
#include <sys/console/console.h>

struct ata_disk_devicedata {
    struct device* device;
    struct ata_controller* controller;
    struct ide_channel* channel;
    struct ata_device* disk;
} __attribute__((packed));

void ata_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");

	panic("ATA read not implemented yet");
}

// BYTE *ata_read(uint64_t start, uint64_t end, uint8_t controller, uint8_t channel, uint8_t device){
// 	ata_select_device(controller, channel, device);
// 	return 0;
// }

void ata_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");

	panic("ATA write not implemented yet");
}

uint16_t ata_sector_size(struct device* dev) {
    panic("Not Implemented");
    return 0;
}
uint32_t ata_total_sectors(struct device* dev) {
    panic("Not Implemented");
    return 0;
}

void device_init_ata_disk(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(dev->deviceData, "deviceData cannot be null");
	struct ata_disk_devicedata* disk = (struct ata_disk_devicedata*) dev->deviceData;

	kprintf("Init %s on controller %s (%s)\n", dev->description, disk->device->name, dev->name);
}

void ata_register_disk(struct device* device, struct ata_controller* controller, struct ide_channel* channel, struct ata_device* disk) {
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &device_init_ata_disk;
	deviceinstance->devicetype=DISK;
	devicemgr_set_device_description(deviceinstance, "ATA Disk");
    /*
	* device data
	*/
	struct ata_disk_devicedata* deviceData = (struct ata_disk_devicedata*) kmalloc(sizeof(struct ata_disk_devicedata));
	deviceData->controller = controller;
	deviceData->channel = channel;
	deviceData->device = device;
	deviceData->disk = disk;
	deviceinstance->deviceData = deviceData;
	/*
    * the device api
    */
    struct deviceapi_block* api = (struct deviceapi_block*) kmalloc(sizeof(struct deviceapi_block));
    api->write = &ata_read;
    api->read = &ata_write;
    api->sector_size = &ata_sector_size;
    api->total_sectors = &ata_total_sectors;
    deviceinstance->api = api;
	/*
	* register
	*/
    devicemgr_register_device(deviceinstance);
}
