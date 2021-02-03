//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all console devices will implement (vga and serial)
 */
#ifndef _DEVICEAPI_CONSOLE_H
#define _DEVICEAPI_CONSOLE_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef uint8_t (*console_setpos_function)(struct device* dev, uint8_t x, uint8_t y);
typedef void (*console_write_function)(struct device* dev, const char* s);

struct deviceapi_console {
    console_setpos_function setpos;
    console_write_function write;
};

#endif