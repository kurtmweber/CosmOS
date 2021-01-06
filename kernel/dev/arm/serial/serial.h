//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARM_SERIAL_H
#define _ARM_SERIAL_H

#include <types.h>

void serial_devicemgr_register_devices();
void serial_write_string(const uint8_t* c);

#endif