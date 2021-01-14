//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _EBDA_H
#define _EBDA_H

#include <types.h>

void ebda_devicemgr_register_devices();

uint16_t ebda_get_rsdp();

#endif