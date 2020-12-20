
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev.h>

/*
* Note that devices are initialized in the order they are registered.
*/
void dev_register_devices(){
	/*
	* scan the PCI bus first
	*/
	pci_init();
	/*
	* set up the pic next
	*/
	pic_register_devices();
	/* 
	* and then RS232
	*/
	serial_register_devices();
	rtc_register_devices();
	keyboard_register_devices();
	display_register_devices();
	usb_register_devices();
	network_register_devices();
	bridge_register_devices();
	ata_register_devices(); 
    mouse_register_devices();
    floppy_register_devices();
    speaker_register_devices();
    pit_register_devices();
}
