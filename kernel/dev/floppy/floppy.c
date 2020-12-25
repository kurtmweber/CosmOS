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
#include <dev/cmos/cmos.h>
#include <sleep/sleep.h>
#include <devicemgr/deviceapi/deviceapi_floppy.h>
#include <panic/panic.h>

#define FLOPPY_IRQ_NUMBER   6
#define FLOPPY_BASE         0x3F0

// https://wiki.osdev.org/Floppy_Disk_Controller#Registers

// registers
#define FLOPPY_STATUS_REGISTER_A                 0x3F0 // read-only
#define FLOPPY_STATUS_REGISTER_B                 0x3F1 // read-only
#define FLOPPY_DIGITAL_OUTPUT_REGISTER           0x3F2 //			 aka (DOR)
#define FLOPPY_TAPE_DRIVE_REGISTER               0x3F3 //			 aka (TSR)
#define FLOPPY_MAIN_STATUS_REGISTER              0x3F4 // read-only  aka (MSR)
#define FLOPPY_DATARATE_SELECT_REGISTER          0x3F4 // write-only aka (DSR)
#define FLOPPY_DATA_FIFO                         0x3F5 //			 aka (FIFO)				
#define FLOPPY_DIGITAL_INPUT_REGISTER            0x3F7 // read-only  aka (DIR)
#define FLOPPY_CONFIGURATION_CONTROL_REGISTER    0x3F7 // write-only aka (CCR)

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
#define FLOPPY_DOR_MOTD_BIT                7 // drive 3 motor (0x80)
#define FLOPPY_DOR_MOTC_BIT                6 // drive 2 motor (0x40)
#define FLOPPY_DOR_MOTB_BIT                5 // drive 1 motor (0x20)
#define FLOPPY_DOR_MOTA_BIT                4 // drive 1 motor (0x10)
#define FLOPPY_DOR_IRQ_BIT                 3 // enable IRQ & DMA (0x08)
#define FLOPPY_DOR_RESET_BIT               2 // reset (0x04) Clear = enter reset mode, Set = normal operation
#define FLOPPY_DOR_DSEL1_BIT               1
#define FLOPPY_DOR_DSEL0_BIT               0

// MSR
#define FLOPPY_MSR_RQM_BIT                 7
#define FLOPPY_MSR_DIO_BIT                 6
#define FLOPPY_MSR_NDMA_BIT                5
#define FLOPPY_MSR_CB_BIT                  4
#define FLOPPY_MSR_ACTD_BIT                3
#define FLOPPY_MSR_ACTC_BIT                2
#define FLOPPY_MSR_ACTB_BIT                1
#define FLOPPY_MSR_ACTA_BIT                0

#define CMOS_FLOPPY_DRIVES_PORT			   0x10

#define FLOPPY_DRIVETYPE_NONE				0x00
#define FLOPPY_DRIVETYPE_360_525			0x01
#define FLOPPY_DRIVETYPE_12_525				0x02
#define FLOPPY_DRIVETYPE_720_35				0x03
#define FLOPPY_DRIVETYPE_144_35				0x04
#define FLOPPY_DRIVETYPE_288_35				0x05

#define FLOPPY_144_SECTORS_PER_TRACK		18

#define FLOPPY_DMA_CHANNEL					2

#define FLOPPY_MOTOR_DELAY_MS_35			300 // 300ms for motor to spin up
#define FLOPPY_MOTOR_DELAY_MS_525			500 // 500ms for motor to spin up

/*
* device parameters for an sb16
*/
struct floppy_devicedata {
    uint32_t port;
} __attribute__((packed));

volatile uint64_t irq_count=0;

void floppy_irq_read(stackFrame *frame) {
	irq_count = irq_count+1;
    kprintf("^");
}

void printDriveType(uint8_t type){
	switch (type) {
		case FLOPPY_DRIVETYPE_360_525:
			kprintf("360 KB 5.25 Drive");
			break;
		case FLOPPY_DRIVETYPE_12_525:
			kprintf("1.2 MB 5.25 Drive");
			break;
		case FLOPPY_DRIVETYPE_720_35:
			kprintf("720 KB 3.5 Drive");
			break;
		case FLOPPY_DRIVETYPE_144_35:
			kprintf("1.44 MB 3.5 Drive");
			break;
		case FLOPPY_DRIVETYPE_288_35:
			kprintf("2.88 MB 3.5 drive");
			break;
	}
}


void resetFloppy() {
	uint64_t current_irq_count = irq_count;
	/*
	* save DOR, write a zero to do a reset.  wait.  write back the DOR.
	*/
	uint8_t currentDOR = asm_in_b(FLOPPY_DIGITAL_OUTPUT_REGISTER);
	asm_out_b(FLOPPY_DIGITAL_OUTPUT_REGISTER,0x00);
	sleep_wait(5);
	asm_out_b(FLOPPY_DIGITAL_OUTPUT_REGISTER,currentDOR);
	/*
	* wait for IRQ
	*/
	while (irq_count < current_irq_count+1){
		sleep_wait(10);
	}
	/*
	* 
	*/
	kprintf("&&");
}


void command(uint8_t commandByte) {
	uint8_t msr_state = asm_in_b(FLOPPY_MAIN_STATUS_REGISTER);
	// check that RQM=1 and DIO=0
	if(((msr_state & 0xc0) != 0x80) ){
		resetFloppy();
	}

	// send command to FIFO
	asm_out_b(FLOPPY_DATA_FIFO, commandByte);
}
/*
* perform device instance specific init here
*/
void deviceInitFloppy(struct device* dev){
	struct floppy_devicedata* deviceData = (struct floppy_devicedata*) dev->deviceData;
    kprintf("Init %s at IRQ %llu (%s)\n",dev->description, FLOPPY_IRQ_NUMBER, dev->name);
	interrupt_router_register_interrupt_handler(FLOPPY_IRQ_NUMBER, &floppy_irq_read);

	uint8_t drives = cmos_read_register(CMOS_FLOPPY_DRIVES_PORT);
	uint8_t master_drives = (drives & 0XF0)>> 4;
	uint8_t slave_drives = (drives & 0X0F);
	
	if (0!=master_drives){
		kprintf("Master Floppy ");
		printDriveType(master_drives);
		kprintf("\n");
	}
	if (0!=slave_drives){
		kprintf("Slave Floppy ");
		printDriveType(slave_drives);
		kprintf("\n");
	}

	// set CCR, DSR to zero
	asm_out_b(FLOPPY_CONFIGURATION_CONTROL_REGISTER ,0x00);
	asm_out_b(FLOPPY_DATARATE_SELECT_REGISTER ,0x00);

	// turn on motor and select drive. enabled IRQS
	asm_out_b(FLOPPY_DIGITAL_OUTPUT_REGISTER | FLOPPY_DOR_IRQ_BIT, FLOPPY_DOR_MOTA_BIT);

	// sleep FLOPPY_MOTOR_DELAY_MS_35 ms to let the motor spin up
	sleep_wait(FLOPPY_MOTOR_DELAY_MS_35);

	// reset
	resetFloppy();
}

/**
 * translate LBA (Logical Block Addressing) to CHS (cylinder, head, sector)
 */
void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector) {
    *cyl    = lba / (2 * FLOPPY_144_SECTORS_PER_TRACK);
    *head   = ((lba % (2 * FLOPPY_144_SECTORS_PER_TRACK)) / FLOPPY_144_SECTORS_PER_TRACK);
    *sector = ((lba % (2 * FLOPPY_144_SECTORS_PER_TRACK)) % FLOPPY_144_SECTORS_PER_TRACK + 1);
}

void floppy_read(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size) {
	panic("Floppy read not implemented yet");
}
void floppy_write(struct device* dev, uint32_t sector, uint8_t* data, uint8_t* size) {
	panic("Floppy write not implemented yet");
}

void floppy_register_floppy(uint64_t port){
   /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	deviceinstance->init =  &deviceInitFloppy;
	deviceinstance->devicetype = FLOPPY;
	devicemgr_set_device_description(deviceinstance, "Floppy");
	/*
    * the device api
    */
    struct deviceapi_floppy* api = (struct deviceapi_floppy*) kmalloc(sizeof(struct deviceapi_floppy));
    api->write = &floppy_read;
    api->read = &floppy_write;
    deviceinstance->api = api;
	/*
	* device data
	*/
	struct floppy_devicedata* deviceData = (struct floppy_devicedata*) kmalloc(sizeof(struct floppy_devicedata));
	deviceData->port = port;
	deviceinstance->deviceData = deviceData;
	/*
	* register
	*/
	devicemgr_register_device(deviceinstance);
}

/**
* find all floppy devices and register them
*/
void floppy_devicemgr_register_devices() {
	floppy_register_floppy(FLOPPY_BASE);
}
