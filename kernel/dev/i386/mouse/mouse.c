//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/mouse/mouse.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/asm/asm.h>
#include <dev/i386/pci/pci.h>
#include <sys/console/console.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/i386/mm/mm.h>
#include <sys/devicemgr/deviceapi/deviceapi_mouse.h>
#include <sys/debug/assert.h>

#define MOUSE_IRQ_NUMBER 12

#define MOUSE_PORT	0x64
#define KB_PORT		0x60

// https://wiki.osdev.org/PS/2_Mouse

#define RESET                   0xFF
#define RESEND                  0xFE
#define SETDEFAULTS             0xF6
#define DISABLE_DATA_REPORTING  0xF5
#define ENABLE_DATA_REPORTING   0xF4
#define SAMPLE_RATE             0xF3
#define DEVICE_ID               0xF2
#define REMOTE_MODE             0xF0
#define WRAP_MODE               0xEE
#define RESET_WRAP_MODE         0xEC
#define READ_DATA               0xEB
#define SET_STREAM_MODE         0xEA
#define STATUS_REQUEST          0xE9
#define SET_RESOLUTION          0xE8
#define SET_SCALING             0xE6

/*
* https://forum.osdev.org/viewtopic.php?t=10247
*/

struct mouse_status* current_mouse_status;

void mouse_irq_read(stackFrame *frame) {
	ASSERT_NOT_NULL(frame, "stackFrame cannot be null");
	ASSERT_NOT_NULL(current_mouse_status, "current_mouse_status cannot be null.  Has the mouse been initialized?");

    kprintf("$");
    switch(current_mouse_status->mouse_cycle) {
    case 0:
        current_mouse_status->mouse_byte[0]=asm_in_b(KB_PORT);
        current_mouse_status->mouse_cycle++;
        break;
    case 1:
        current_mouse_status->mouse_byte[1]=asm_in_b(KB_PORT);
        current_mouse_status->mouse_cycle++;
        break;
    case 2:
        current_mouse_status->mouse_byte[2]=asm_in_b(KB_PORT);
        current_mouse_status->mouse_x=current_mouse_status->mouse_byte[1];
        current_mouse_status->mouse_y=current_mouse_status->mouse_byte[2];
        current_mouse_status->mouse_cycle=0;
        break;
    }
}

void mouse_wait(uint8_t a_type) {
    uint32_t time_out=100000; 
    if(a_type==0) {
        while(time_out--) {
            if((asm_in_b(MOUSE_PORT) & 1)==1) {
                return;
            }
        }
    return;
    } else {
        while(time_out--) {
            if((asm_in_b(MOUSE_PORT) & 2)==0) {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8_t a_write){
  // wait to be able to send a command
  mouse_wait(1);
  // tell the mouse we are sending a command
  asm_out_b(MOUSE_PORT, 0xD4);
  // wait for the final part
  mouse_wait(1);
  // finally write
  asm_out_b(KB_PORT, a_write);
}

uint8_t mouse_read() {
  // gets response from mouse
  mouse_wait(0);
  return asm_in_b(KB_PORT);
}

/*
* perform device instance specific init here
*/
void mouse_device_init(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s at IRQ %llu (%s)\n",dev->description, MOUSE_IRQ_NUMBER, dev->name);
    interrupt_router_register_interrupt_handler(MOUSE_IRQ_NUMBER, &mouse_irq_read);

    // alloc struct
    current_mouse_status = kmalloc(sizeof(struct mouse_status));
    current_mouse_status->mouse_cycle=0;

    // enable aux mouse
    mouse_wait(1);
    asm_out_b(MOUSE_PORT, 0xa8);
    
     //Enable the interrupts
    mouse_wait(1);
    asm_out_b(MOUSE_PORT, 0x20);
    mouse_wait(0);
    uint8_t _status=(asm_in_b(KB_PORT) | 2);
    mouse_wait(1);
    asm_out_b(MOUSE_PORT, 0x60);
    mouse_wait(1);
    asm_out_b(KB_PORT, _status);
 
    // tell the mouse to use default settings
    mouse_write(SETDEFAULTS);
    mouse_read();  //Acknowledge
 
    // enable the mouse
    mouse_write(ENABLE_DATA_REPORTING);
    mouse_read();  //Acknowledge
}

struct mouse_status* ps2mouse_status(struct device* dev) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
	ASSERT_NOT_NULL(current_mouse_status, "current_mouse_status cannot be null.  Has the mouse been initialized?");
    return current_mouse_status;
}

/**
* find all PS/2 mouse devices and register them
*/
void mouse_devicemgr_register_devices() {
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	deviceinstance->init =  &mouse_device_init;
	deviceinstance->devicetype = MOUSE;
	devicemgr_set_device_description(deviceinstance, "PS2 Mouse");
	/*
	* device api
	*/
	struct deviceapi_mouse* api = (struct deviceapi_mouse*) kmalloc (sizeof(struct deviceapi_mouse));
	api->status = &ps2mouse_status;
	deviceinstance->api = api;
	/*
	* register
	*/
	devicemgr_register_device(deviceinstance);
}
