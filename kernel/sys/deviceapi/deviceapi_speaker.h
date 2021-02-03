//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all speaker devices will implement
 */
#ifndef _DEVICEAPI_SPEAKER_H
#define _DEVICEAPI_SPEAKER_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

typedef void (*speaker_beep_function)(struct device* dev, uint32_t frequency, uint32_t milliseconds);

struct deviceapi_speaker {
    speaker_beep_function beep;
};

#endif