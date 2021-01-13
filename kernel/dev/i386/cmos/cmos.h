//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CMOS_H
#define _CMOS_H

#include <types.h>

#define CMOS_REGISTER_SELECT_PORT 0x70
#define CMOS_REGISTER_DATA_PORT 0x71

void cmos_devicemgr_register_devices();

void cmos_write_register(uint8_t reg, uint8_t val);
uint8_t cmos_read_register(uint8_t reg);

#endif