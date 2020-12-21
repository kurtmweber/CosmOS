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

// registers
#define FLOPPY_STATUS_REGISTER_A                 0x3F0 // read-only
#define FLOPPY_STATUS_REGISTER_B                 0x3F1 // read-only
#define FLOPPY_DIGITAL_OUTPUT_REGISTER           0x3F2
#define FLOPPY_TAPE_DRIVE_REGISTER               0x3F3
#define FLOPPY_MAIN_STATUS_REGISTER              0x3F4 // read-only
#define FLOPPY_DATARATE_SELECT_REGISTER          0x3F4 // write-only
#define FLOPPY_DATA_FIFO                         0x3F5
#define FLOPPY_DIGITAL_INPUT_REGISTER            0x3F7 // read-only
#define FLOPPY_CONFIGURATION_CONTROL_REGISTER    0x3F7  // write-only

// commands
#define FLOPPY_READ_TRACK                  2	// generates IRQ6
#define FLOPPY_SPECIFY                     3      // * set drive parameters
#define FLOPPY_SENSE_DRIVE_STATUS          4
#define FLOPPY_WRITE_DATA                  5      // * write to the disk
#define FLOPPY_READ_DATA                   6      // * read from the disk
#define FLOPPY_RECALIBRATE                 7      // * seek to cylinder 0
#define FLOPPY_SENSE_INTERRUPT             8      // * ack IRQ6 get status of last command
#define FLOPPY_WRITE_DELETED_DATA          9
#define FLOPPY_READ_ID                     10	// generates IRQ6
#define FLOPPY_READ_DELETED_DATA           12
#define FLOPPY_FORMAT_TRACK                13     // *
#define FLOPPY_DUMPREG                     14
#define FLOPPY_SEEK                        15     // * seek both heads to cylinder X
#define FLOPPY_VERSION                     16	// * used during initialization, once
#define FLOPPY_SCAN_EQUAL                  17
#define FLOPPY_PERPENDICULAR_MODE          18	// * used during initialization, once, maybe
#define FLOPPY_CONFIGURE                   19     // * set controller parameters
#define FLOPPY_LOCK                        20     // * protect controller params from a reset
#define FLOPPY_VERIFY                      22
#define FLOPPY_SCAN_LOW_OR_EQUAL           25
#define FLOPPY_SCAN_HIGH_OR_EQUAL          29

// DOR
#define FLOPPY_DOR_MOTD_BIT                7
#define FLOPPY_DOR_MOTC_BIT                6
#define FLOPPY_DOR_MOTB_BIT                5
#define FLOPPY_DOR_MOTA_BIT                4
#define FLOPPY_DOR_IRQ_BIT                 3
#define FLOPPY_DOR_RESET_BIT               2
#define FLOPPY_DOR_DSEL1 _BIT              1
#define FLOPPY_DOR_DSEL0 _BIT              0

// MSR
#define FLOPPY_MSR_RQM_BIT                 7
#define FLOPPY_MSR_DIO_BIT                 6
#define FLOPPY_MSR_NDMA_BIT                5
#define FLOPPY_MSR_CB_BIT                  4
#define FLOPPY_MSR_ACTD_BIT                3
#define FLOPPY_MSR_ACTC_BIT                2
#define FLOPPY_MSR_ACTB_BIT                1
#define FLOPPY_MSR_ACTA_BIT                0

void floppy_irq_read(stackFrame *frame) {

}

/*
* perform device instance specific init here
*/
void deviceInitFloppy(struct device* dev){
    kprintf("Init %s at IRQ %llu\n",dev->description, FLOPPY_IRQ_NUMBER);
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
