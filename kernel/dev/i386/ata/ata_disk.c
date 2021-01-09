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
#include <sys/sleep/sleep.h>
#include <sys/asm/byte.h>

// https://wiki.osdev.org/PCI_IDE_Controller

struct ata_disk_devicedata {
    struct device* device;
    struct ata_controller* controller;
    uint8_t channel;
    uint8_t disk;
} __attribute__((packed));

// registers needs to be uint8_t[6]
void calculate_ida_lba_register_values( uint32_t lba, uint8_t* registers) {
	registers[0]=(lba & 0x000000FF) >> 0;
	registers[1]=(lba & 0x0000FF00) >> 8;
	registers[2]=(lba & 0x00FF0000) >> 16;
	registers[3]=(lba & 0xFF000000) >> 24;
	registers[4]=0;
	registers[5]=0;
}

void ata_rw(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count, bool read) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(data, "data cannot be null");
	struct ata_disk_devicedata* diskdata = (struct ata_disk_devicedata*) dev->deviceData;
	struct ata_device* disk = ata_get_disk(diskdata->device, diskdata->channel, diskdata->disk);
	uint16_t sector_size = disk->bytes_per_sector;

//	kprintf("channel %llu \n", diskdata->channel);
//	kprintf("disk %llu \n", diskdata->disk);
	ata_select_device(diskdata->controller, diskdata->channel, diskdata->disk);

	// wait
	ata_wait_busy(diskdata->controller, diskdata->channel);

	// sector count
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_SECTOR_COUNT_1, 0);		

	// lba
	uint8_t regs[6];
	calculate_ida_lba_register_values(sector, regs);
//	kprintf("sector %llu \n", sector);
//	kprintf("regs %llu %llu %llu %llu %llu %llu\n", regs[0],regs[1],regs[2],regs[3],regs[4],regs[5]);
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_3, regs[3]);		
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_4, regs[4]);		
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_5, regs[5]);		

	// sector count
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_SECTOR_COUNT_0, count);		
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_0, regs[0]);		
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_1, regs[1]);		
	ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_LBA_2, regs[2]);		

	// select master
	/*
	*	Bits 0 : 3: Head Number for CHS.
	*	Bit 4: Slave Bit. (0: Selecting Master Drive, 1: Selecting Slave Drive).
	*	Bit 5: Obsolete and isn't used, but should be set.
	*	Bit 6: LBA (0: CHS, 1: LBA).
	*	Bit 7: Obsolete and isn't used, but should be set.
	*/
	// E0 is bits 5,6,7 set.
	if (diskdata->disk==0) {
		// master
		ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_HDDEVSEL,  0xE0);
	} else {
		// slave
		ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_HDDEVSEL,  0xE0 | 0x10);
	}

	if (read==true) { 
		// send the read command
		ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_COMMAND, ATA_CMD_READ_PIO);	
	} else {
		// send the write command
		ata_register_write(diskdata->controller, diskdata->channel,ATA_REGISTER_COMMAND, ATA_CMD_WRITE_PIO);	
	}	

	while(ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS)& ATA_STATUS_BUSY) {
		sleep_wait(1);
	}

//	kprintf("fff %llu\n",ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS) & ATA_STATUS_BUSY);

	uint8_t status = ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS);
//	kprintf("Status: %llu\n", status);
	if (status & ATA_STATUS_ERROR){
		kprintf("IDE Error\n");
	} 
	if (status & ATA_STATUS_BUSY){
		kprintf("IDE Busy\n");
	} 

	uint16_t* buffer = (uint16_t*) data;

	uint32_t idx=0;
	for (int j =0;j<count;j++) {
		ata_wait_busy(diskdata->controller, diskdata->channel);
		ata_wait_drq(diskdata->controller, diskdata->channel);
		for(int i=0;i<sector_size/2;i++) {
			if (read == true) {
				buffer[idx++]=ata_register_read_word(diskdata->controller, diskdata->channel,ATA_REGISTER_DATA);
			} else {
				ata_register_write_word(diskdata->controller, diskdata->channel,ATA_REGISTER_DATA, buffer[idx++]);
			}
		}
	}
}

void ata_read(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
	ata_rw(dev, sector, data, count, true);
}

void ata_write(struct device* dev, uint32_t sector, uint8_t* data, uint32_t count) {
	ata_rw(dev, sector, data, count, false);
}

uint16_t ata_sector_size(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct ata_disk_devicedata* diskdata = (struct ata_disk_devicedata*) dev->deviceData;
	struct ata_device* disk = ata_get_disk(diskdata->device, diskdata->channel, diskdata->disk);
	ASSERT_NOT_NULL(disk, "disk should not be null");
	return disk->bytes_per_sector;
}

uint32_t ata_total_size(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	struct ata_disk_devicedata* diskdata = (struct ata_disk_devicedata*) dev->deviceData;
	struct ata_device* disk = ata_get_disk(diskdata->device, diskdata->channel, diskdata->disk);
	ASSERT_NOT_NULL(disk, "disk should not be null");
	return disk->size;
}

void device_init_ata_disk(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(dev->deviceData, "deviceData cannot be null");
	struct ata_disk_devicedata* disk = (struct ata_disk_devicedata*) dev->deviceData;
	struct ata_device* dsk = ata_get_disk(disk->device, disk->channel, disk->disk);
	ASSERT_NOT_NULL(dsk, "dsk should not be null");

	kprintf("Init %s serial '%s' on controller %s of size %llu (%s)\n", dev->description,  dsk->serial, disk->device->name, dsk->size, dev->name);
}

void ata_register_disk(struct device* controllerDevice, uint8_t channel, uint8_t disk) {
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
	deviceData->controller = controllerDevice->deviceData;
	deviceData->channel = channel;
	deviceData->device = controllerDevice;
	deviceData->disk = disk;
	deviceinstance->deviceData = deviceData;
	/*
    * the device api
    */
    struct deviceapi_block* api = (struct deviceapi_block*) kmalloc(sizeof(struct deviceapi_block));
    api->write = &ata_write;
    api->read = &ata_read;
    api->sector_size = &ata_sector_size;
    api->total_size = &ata_total_size;
    deviceinstance->api = api;
	/*
	* register
	*/
    devicemgr_register_device(deviceinstance);
}
