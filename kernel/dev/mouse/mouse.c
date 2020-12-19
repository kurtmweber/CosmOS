//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/mouse/mouse.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

#define MOUSE_IRQ_NUMBER 12

#define MOUSE_PORT	0x64
#define KB_PORT		0x60

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

uint8_t mouse_cycle=0;
int8_t mouse_byte[3];
int8_t mouse_x=0;
int8_t mouse_y=0;


void mouse_irq_read(stackFrame *frame) {
    kprintf("$");
    switch(mouse_cycle) {
    case 0:
        mouse_byte[0]=asm_in_b(KB_PORT);
        mouse_cycle++;
        break;
    case 1:
        mouse_byte[1]=asm_in_b(KB_PORT);
        mouse_cycle++;
        break;
    case 2:
        mouse_byte[2]=asm_in_b(KB_PORT);
        mouse_x=mouse_byte[1];
        mouse_y=mouse_byte[2];
        mouse_cycle=0;
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
  //Finally write
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
void deviceInitMouse(struct device* dev){
    kprintf("Init %s at IRQ %llu\n",dev->description, MOUSE_IRQ_NUMBER);
    registerInterruptHandler(MOUSE_IRQ_NUMBER, &mouse_irq_read);

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

/**
* find all PS/2 mouse devices and register them
*/
void mouse_register_devices() {
    /*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceinstance->init =  &deviceInitMouse;
	deviceinstance->devicetype = MOUSE;
	deviceSetDescription(deviceinstance, "PS2 Mouse");
	registerDevice(deviceinstance);
}
