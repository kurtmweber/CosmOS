//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this file defines the interface that all DSP devices will implement
*/
#ifndef _DEVICEAPI_DSP_H
#define _DEVICEAPI_DSP_H

#include <types.h>
#include <devicemgr/devicemgr.h>

typedef void (*dsp_play_function)(struct device* dev, uint8_t* buffer, uint64_t len);

struct deviceapi_dsp {
    dsp_play_function play;
};

#endif