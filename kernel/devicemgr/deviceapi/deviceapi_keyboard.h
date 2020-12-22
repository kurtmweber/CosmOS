//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all keyboard devices will implement
*/
#ifndef _DEVICEAPI_KEYBOARD_H
#define _DEVICEAPI_KEYBOARD_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef uint8_t (*keyboard_read_key)(struct device* dev);

struct deviceapi_keyboard {
    keyboard_read_key read;
};

#endif