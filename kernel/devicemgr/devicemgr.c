//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <collection/list/list.h>
#include <mm/mm.h>
#include <string/string.h>
#include <debug/assert.h>
#include <devicemgr/deviceregistry.h>
#include <dev/dev.h>

#define MAX_DEVICE_NAME_LENGTH 64

int8_t* DeviceTypeNames[] = {"None"
	,"serial"
	,"vga"
	,"rtc"
	,"keyboard"
	,"nic"
	,"bridge"
	,"usb"
	,"ata"
	,"pic"
	,"mouse"
	,"floppy"
	,"speaker"
	,"pit"
	,"dsp"
    ,"cmos"
    ,"dma"
    ,"cpu"
    ,"rd"
    ,"vnic"
    ,"vblock"
    }; 

void devicemgr_init() {
    deviceregistry_init();    
}

int8_t* createDeviceName(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    int8_t nn[32];
    int8_t* ret = kmalloc(MAX_DEVICE_NAME_LENGTH);
    strcpy(ret, DeviceTypeNames[dev->devicetype]);
    uitoa3(dev->type_index, nn, 32, 10);
    ret = strcat(ret, nn);
    return ret;
}

void devicemgr_register_device(struct device* dev) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    ASSERT_NOT_NULL(dev->description, "description cannot be null");
    ASSERT_NOT_NULL(dev->devicetype, "devicetype cannot be null");
    ASSERT_NOT_NULL(dev->init, "init cannot be null");
    /*
    * set index
    */
    dev->type_index = deviceregistry_devicecount_type(dev->devicetype);
    /*
    * create name
    */
    dev->name = createDeviceName(dev);
    /*
    * register
    */
    deviceregistry_registerdevice(dev);
}

uint16_t devicemgr_device_count() {
    return deviceregistry_devicecount();
}

void deviceInitIterator(struct device* dev) {
    if (0!=dev) {
        dev->init(dev);
    } else {
        panic("um. why is there a null device?");
    }
}

/*
* init order matters
*/
void devicemgr_init_devices(){
    kprintf("Initializing Devices\n");
    /*
    * CPU first before first?
    */
    deviceregistry_iterate_type(CPU, deviceInitIterator);
    /*
    * PIC first
    */
    deviceregistry_iterate_type(PIC, deviceInitIterator);
    /*
    * Serial next
    */
    deviceregistry_iterate_type(SERIAL, deviceInitIterator);
   /*
    * the PIT
    */
    deviceregistry_iterate_type(PIT, deviceInitIterator);
    /*
    * CMOS
    */
    deviceregistry_iterate_type(CMOS, deviceInitIterator);
    /*
    * DMA
    */
    deviceregistry_iterate_type(DMA, deviceInitIterator);
    /*
    * virtual devices
    */ 
    deviceregistry_iterate_type(VNIC, deviceInitIterator);
    deviceregistry_iterate_type(VBLOCK, deviceInitIterator);
    /*
    * everything else
    */
    deviceregistry_iterate_type(RTC, deviceInitIterator);
    deviceregistry_iterate_type(KEYBOARD, deviceInitIterator); 
    deviceregistry_iterate_type(VGA, deviceInitIterator);
    deviceregistry_iterate_type(USB, deviceInitIterator);
    deviceregistry_iterate_type(NIC, deviceInitIterator);
    deviceregistry_iterate_type(BRIDGE, deviceInitIterator);
    deviceregistry_iterate_type(ATA, deviceInitIterator);
    deviceregistry_iterate_type(MOUSE, deviceInitIterator);
    deviceregistry_iterate_type(FLOPPY, deviceInitIterator);
    deviceregistry_iterate_type(SPEAKER, deviceInitIterator);
    deviceregistry_iterate_type(DSP, deviceInitIterator);
    deviceregistry_iterate_type(RAMDISK, deviceInitIterator);
}

struct device* devicemgr_new_device() {
    struct device* ret= (struct device*) kmalloc(sizeof(struct device));
    ret->description=0;
    ret->init=0;
    ret->deviceData=0;
    ret->name=0;
    ret->type_index=0;
    ret->devicetype=0;
    ret->api=0;
    ret->pci=0;
    return ret;
}

void devicemgr_set_device_description(struct device* dev, int8_t* description) {
    ASSERT_NOT_NULL(dev, "dev cannot be null");
    ASSERT_NOT_NULL(description, "description cannot be null");
    uint32_t size = strlen(description);
    if (0!=dev->description){
        kfree(dev->description);
    }
    dev->description = kmalloc(size+1);
    strcpy(dev->description, description);
}

struct device* devicemgr_findDevice(const int8_t* name) {
    ASSERT_NOT_NULL(name, "name cannot be null");
    return deviceregistry_findDevice(name);
}

void devicemgr_register_devices() {
	/*
	* scan the PCI bus first
	*/
	pci_init();
	/*
	* register up the pic next
	*/
	pic_devicemgr_register_devices();
	/* 
	* and then RS232
	*/
	serial_devicemgr_register_devices();
	/*
	* and the then the PIT
	*/
	pit_devicemgr_register_devices();
	/*
	* we need the CMOS
	*/
	cmos_devicemgr_register_devices();
	/*
	* ISA DMA Controller
	*/
	isadma_devicemgr_register_devices();
	/*
	* rest of this stuff can really happen in any order
	*/
	rtc_devicemgr_register_devices();
	keyboard_devicemgr_register_devices();
	display_devicemgr_register_devices();
	usb_devicemgr_register_devices();
	network_devicemgr_register_devices();
	bridge_devicemgr_register_devices();
	ata_devicemgr_register_devices(); 
    mouse_devicemgr_register_devices();
   // floppy_devicemgr_register_devices();
    speaker_devicemgr_register_devices();
	sb16_devicemgr_register_devices();
//	ac97_devicemgr_register_devices();
//	adlib_devicemgr_register_devices();
    cpu_devicemgr_register_devices();
    virtio_devicemgr_register_devices();
    ramdisk_devicemgr_register_devices();
    ehcibus_devicemgr_register_devices();
}




