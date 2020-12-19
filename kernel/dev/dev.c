
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev.h>

/*
* Note that devices are initialized in the order they are registered.  So PIC first....
*/
void dev_register_devices(){
	pic_register_devices();
	serial_register_devices();
	rtc_register_devices();
	keyboard_register_devices();
	display_register_devices();
	usb_register_devices();
	network_register_devices();
	bridge_register_devices();
	ata_register_devices(); 
    mouse_register_devices();
}
