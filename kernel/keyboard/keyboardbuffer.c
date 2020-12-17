/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>
#include <keyboard/keyboard.h>

void keyboard_buffer_add(key_action_t add){
	keyboard_buffer[keyboard_buffer_end] = add;
	
	if (keyboard_buffer_end == 255){
		keyboard_buffer_end = 0;
	} else {
		keyboard_buffer_end++;
	}
	
	if (keyboard_buffer_end == keyboard_buffer_start){
		if (keyboard_buffer_start == 255){
			keyboard_buffer_start = 0;
		} else {
			keyboard_buffer_start++;
		}
	}
	
	return;
}

key_action_t keyboard_buffer_consume(void){
	uint8_t i;
	
	if ((keyboard_buffer_end == 0) && (keyboard_buffer_start == 0)){
		keyboard_buffer[0].key = 0xFF;
		keyboard_buffer[0].state = KEYPRESS_MAKE;	// not strictly necessary since it should never be checked when .key == 0xFF, but just to leave it in a defined state
		
		return keyboard_buffer[0];
	}
	
	i = keyboard_buffer_start;	// we need this so we can return keyboard_buffer[i] in case we reset keyboard_buffer_start and keyboard_buffer_end to 0 if we reach the end
	
	if (keyboard_buffer_start == 255){
		keyboard_buffer_start = 0;
	} else {
		keyboard_buffer_start++;
	}
	
	if (keyboard_buffer_start == keyboard_buffer_end){
		keyboard_buffer_start = 0;
		keyboard_buffer_end = 0;
	}
	
	return keyboard_buffer[i];
}
