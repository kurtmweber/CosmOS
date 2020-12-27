//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all Serial devices will implement
*/
#ifndef _DEVICEAPI_SERIAL_H
#define _DEVICEAPI_SERIAL_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef void (*serial_write_function)(struct device* dev, const int8_t* c);

struct deviceapi_serial {
    serial_write_function write;
};

#endif