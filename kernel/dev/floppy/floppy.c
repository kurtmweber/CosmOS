//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/floppy/floppy.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

#define FLOPPY_IRQ_NUMBER 6

// https://wiki.osdev.org/Floppy_Disk_Controller#Registers

enum FloppyRegisters {
   STATUS_REGISTER_A                = 0x3F0, // read-only
   STATUS_REGISTER_B                = 0x3F1, // read-only
   DIGITAL_OUTPUT_REGISTER          = 0x3F2,
   TAPE_DRIVE_REGISTER              = 0x3F3,
   MAIN_STATUS_REGISTER             = 0x3F4, // read-only
   DATARATE_SELECT_REGISTER         = 0x3F4, // write-only
   DATA_FIFO                        = 0x3F5,
   DIGITAL_INPUT_REGISTER           = 0x3F7, // read-only
   CONFIGURATION_CONTROL_REGISTER   = 0x3F7  // write-only
};

enum FloppyCommands {
   READ_TRACK =                 2,	// generates IRQ6
   SPECIFY =                    3,      // * set drive parameters
   SENSE_DRIVE_STATUS =         4,
   WRITE_DATA =                 5,      // * write to the disk
   READ_DATA =                  6,      // * read from the disk
   RECALIBRATE =                7,      // * seek to cylinder 0
   SENSE_INTERRUPT =            8,      // * ack IRQ6, get status of last command
   WRITE_DELETED_DATA =         9,
   READ_ID =                    10,	// generates IRQ6
   READ_DELETED_DATA =          12,
   FORMAT_TRACK =               13,     // *
   DUMPREG =                    14,
   SEEK =                       15,     // * seek both heads to cylinder X
   VERSION =                    16,	// * used during initialization, once
   SCAN_EQUAL =                 17,
   PERPENDICULAR_MODE =         18,	// * used during initialization, once, maybe
   CONFIGURE =                  19,     // * set controller parameters
   LOCK =                       20,     // * protect controller params from a reset
   VERIFY =                     22,
   SCAN_LOW_OR_EQUAL =          25,
   SCAN_HIGH_OR_EQUAL =         29
};

void floppy_irq_read(stackFrame *frame) {

}

/*
* perform device instance specific init here
*/
void deviceInitFloppy(struct device* dev){
    kprintf("Init %s at IRQ %llu (%s)\n",dev->description, FLOPPY_IRQ_NUMBER, dev->name);
	interrupt_router_register_interrupt_handler(FLOPPY_IRQ_NUMBER, &floppy_irq_read);
}

/**
* find all floppy devices and register them
*/
void floppy_devicemgr_register_devices() {
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	deviceinstance->init =  &deviceInitFloppy;
	deviceinstance->devicetype = FLOPPY;
	devicemgr_set_device_description(deviceinstance, "Floppy");
	devicemgr_register_device(deviceinstance);
}
