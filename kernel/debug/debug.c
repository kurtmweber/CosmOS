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

#define DEBUG_LINE_WIDTH 8

uint8_t CHAR_ARRAY[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'} ;

void debug_show_memblock(uint8_t* block, uint16_t size) {
    uint8_t out[16];

    for (uint16_t i=0; i<size; i++){
        uint16_t idx=0;
        out[idx++]='0';
        out[idx++]='x';
        uint8_t byte = block[i];
        uint8_t high = (byte & 0xF0)>>4;
        out[idx++]=CHAR_ARRAY[high];
        uint8_t low = byte & 0x0F;
        out[idx++]=CHAR_ARRAY[low];
        out[idx++]=' ';
        if (0 == ((i+1) % (uint8_t)DEBUG_LINE_WIDTH)){
            out[idx++]='\n';    
        }
        out[idx++]=0;
        serial_write_string(out);
    }
}
