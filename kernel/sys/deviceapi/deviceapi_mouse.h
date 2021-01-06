//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all mouse devices will implement
*/
#ifndef _DEVICEAPI_MOUSE_H
#define _DEVICEAPI_MOUSE_H

#include <types.h>
#include <sys/devicemgr/devicemgr.h>

struct mouse_status {
    uint8_t mouse_cycle;
    int8_t mouse_byte[3];
    int8_t mouse_x;
    int8_t mouse_y;
};

typedef struct mouse_status* (*mouse_status_function)(struct device* dev);

struct deviceapi_mouse {
    mouse_status_function status;
};

#endif