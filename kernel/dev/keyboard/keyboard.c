/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <asm/asm.h>
#include <console/drivers/drivers.h>
#include <dev/keyboard/keyboard.h>
#include <console/console.h>
#include <interrupts/interrupts.h>
#include <interrupts/interrupt_router.h>
#include <devicemgr/devicemgr.h>
#include <dev/keyboard/abstract_keyboard.h>

#define KB_IRQ_NUMBER 1

#define KBD_PORT		0x60
#define KBD_CTRL_PORT		0x64

#define KBD_ACK			0xFA
#define KBD_ECHO_RESPONSE	0xEE
#define KBD_ERROR_1		0x00
#define KBD_ERROR_2		0xFF
#define KBD_RESEND		0xFE
#define KBD_TEST_FAILED_1	0xFC
#define KBD_TEST_FAILED_2	0xFD
#define KBD_TEST_PASSED		0xAA

key_action_t keyboard_buffer[256];
uint8_t keyboard_buffer_start, keyboard_buffer_end;

void keyboard_add_command_queue(uint8_t command){
}

void keyboard_irq_read(stackFrame *frame){
	kprintf(".");
	uint8_t read_byte;
	static bool long_scan_code = false, longer_scan_code = false;
	static bool prnt_scrn_scan_code = false;
	key_action_t keypress;
	
	read_byte = asm_in_b(KBD_PORT);
	
	// scan codes for the Print Screen key are 0xE0, 0x2A, 0xE0, 0x37 (make) and 0xE0, 0xB7, 0xE0, 0xAA (break)
	// Since neither 0x2A nor 0xB7 are, by themselves, scan codes in the two-byte 0xE0 class, once we've received them
	// after an initial 0xE0, we can discard them, as well as the next scan code (which will be a 0xE0 and just look at
	// the final scan code to determine whether it's a make or break action.
	
	if (prnt_scrn_scan_code){
		if (read_byte == 0x37){
			keypress.key = P(0,13);
			keypress.state = KEYPRESS_MAKE;
			keyboard_buffer_add(keypress);
			
			prnt_scrn_scan_code = false;	// done processing scan codes for print screen
		}
		
		if ((read_byte = 0xAA)){
			keypress.key = P(0,13);
			keypress.state = KEYPRESS_BREAK;
			keyboard_buffer_add(keypress);
			
			prnt_scrn_scan_code = false;
		}
		
		// if it's neither a 0x37 or 0xAA, then it's the second 0xE0 of the print screen scan code sequence,
		// so we just return, effectively discarding it
		
		return;
	}
	
	if (long_scan_code){	// we're on the second byte of a 0xE0 XX scan long_scan_code
		// first we want to make sure it's not a prnt scrn press or release
		if ((read_byte == 0x2A) || (read_byte == 0xB7)){
			prnt_scrn_scan_code = true;
			
			// reset long_scan_code so we're prepared for the start of the next scan code
			// it being false won't be an issue because prnt_scrn_scan_code is set, so we'll
			// return before we even get to that point
			long_scan_code = false;
			
			// we don't actually need this (see above), so discard it by returning
			return;
		}
		
		// otherwise, it's a normal two-byte scancode
		
		if (read_byte >= 0x80){		// we have a BREAK scancode
			keypress.key = abstract_keyboard_grid_2byte[read_byte - 0x80];
			keypress.state = KEYPRESS_BREAK;
		} else {
			keypress.key = abstract_keyboard_grid_2byte[read_byte];
			keypress.state = KEYPRESS_MAKE;
		}
		
		keyboard_buffer_add(keypress);
		
		long_scan_code = false;
		
		return;
	}
	
	// basically just the PAUSE key, which does NOT send separate scan codes for make and break - instead, a single interrupt is generated
	// on press, and it is treated as pressed and released instantaneously
	// but the process is essentially the same as for Print Screen
	if (longer_scan_code){
		if (read_byte == 0xC5){
			keypress.key = P(0,15);
			keypress.state = KEYPRESS_MAKE;
			keyboard_buffer_add(keypress);
			keypress.key = P(0,15);
			keypress.state = KEYPRESS_BREAK;
			keyboard_buffer_add(keypress);
			
			longer_scan_code = false;
		}
		
		return;
	}
	
	switch (read_byte){
		case KBD_ACK:
			keyboard_remove_command_queue();
			break;
		case KBD_ECHO_RESPONSE:
			break;
		case KBD_ERROR_1:
		case KBD_ERROR_2:
			break;
		case KBD_RESEND:
			keyboard_send_command_queue();
			break;
		case KBD_TEST_FAILED_1:
		case KBD_TEST_FAILED_2:
			break;
		case KBD_TEST_PASSED:
			break;
		case 0xE0:
			long_scan_code = true;
			break;
		case 0xE1:
			longer_scan_code = true;
			break;
		default:
			if (read_byte >= 0x80){
				keypress.key = abstract_keyboard_grid[read_byte - 0x80];
				keypress.state = KEYPRESS_BREAK;
				keyboard_buffer_add(keypress);
			} else {
				keypress.key = abstract_keyboard_grid[read_byte];
				keypress.state = KEYPRESS_MAKE;
				keyboard_buffer_add(keypress);
			}
			
			break;
	}
	
	return;
}

void keyboard_remove_command_queue(){
}

void keyboard_send_command_queue(){
}

/*
* perform device instance specific init here
*/
void deviceInitKeyboard(struct device* dev){
	struct pci_device* pci_dev = (struct pci_device*) dev->deviceData;
    kprintf("Init %s at IRQ %llu\n",dev->description, KB_IRQ_NUMBER);
	registerInterruptHandler(KB_IRQ_NUMBER, &keyboard_irq_read);
}

/**
* find all keyboard devices and register them
*/
void keyboard_register_devices(){
	keyboard_buffer_start = 0;
	keyboard_buffer_end = 0;

	/*
	* register device
	*/
	struct device* deviceinstance = newDevice();
	deviceinstance->init =  &deviceInitKeyboard;
	deviceinstance->devicetype = KEYBOARD;
	deviceSetDescription(deviceinstance, "Keyboard");
	registerDevice(deviceinstance);
}


