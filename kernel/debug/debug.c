//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <debug/debug.h>

// normally this is not encouraged and instead the devivemgr should be used
// however, for debug, this might actually be a better approach
#include <dev/serial/serial.h>

#define DEBUG_LINE_WIDTH 20

uint8_t CHAR_ARRAY[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'} ;

void debug_show_memblock(uint8_t* block, uint16_t size) {
    uint8_t out[4];
    for (uint16_t i=0; i<size; i++){
        uint8_t byte = block[i];
        uint8_t high = (byte & 0xF0)>>4;
        out[0]=CHAR_ARRAY[high];
        uint8_t low = byte & 0x0F;
        out[1]=CHAR_ARRAY[low];
        out[2]=' ';
        out[3]=0;
    }
}
