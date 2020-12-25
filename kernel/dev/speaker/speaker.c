/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/speaker/speaker.h>
#include <asm/asm.h>
#include <devicemgr/devicemgr.h>
#include <console/console.h>
#include <sleep/sleep.h>
#include <devicemgr/deviceapi/deviceapi_speaker.h>
#include <panic/panic.h>

// https://wiki.osdev.org/PC_Speaker

#define SPEAKER_PORT    0x61

/*
* perform device instance specific init here
*/
void deviceInitSpeaker(struct device* dev){
	ASSERT_NOT_NULL(dev, "dev cannot be null");
    kprintf("Init %s (%s)\n" ,dev->description, dev->name);
}

 //make it shutup
 static void nosound() {
 	uint8_t tmp = asm_in_b(SPEAKER_PORT) & 0xFC;
 	asm_out_b(SPEAKER_PORT, tmp);
 }
 
void play_sound(uint32_t frequency) {
 	uint32_t div;
 	uint8_t tmp;
 
    // set the PIT to the desired frequency
 	div = 1193180 / frequency;
 	asm_out_b(0x43, 0xb6);
 	asm_out_b(0x42, (uint8_t) (div) );
 	asm_out_b(0x42, (uint8_t) (div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = asm_in_b(SPEAKER_PORT);
  	if (tmp != (tmp | 3)) {
 		asm_out_b(SPEAKER_PORT, tmp | 3);
 	}
 }

 //Make a beep
 void speaker_beep(struct device* dev, uint32_t frequency, uint32_t milliseconds) {
	ASSERT_NOT_NULL(dev, "dev cannot be null");
 	 play_sound(frequency);
 	 sleep_wait(milliseconds);
 	 nosound();
          //set_PIT_2(old_frequency);
 }

void speaker_devicemgr_register_devices(){
    /*
	* register device
	*/
	struct device* deviceinstance = devicemgr_new_device();
	devicemgr_set_device_description(deviceinstance, "Speaker");
	deviceinstance->devicetype = SPEAKER;
	deviceinstance->init =  &deviceInitSpeaker;
	/*
	* device api
	*/
	struct deviceapi_speaker* api = (struct deviceapi_speaker*) kmalloc (sizeof(struct deviceapi_speaker));
	api->beep = &speaker_beep;
	deviceinstance->api = api;
	/**
	* register
	*/
	devicemgr_register_device(deviceinstance);
}




