//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PL101_H
#define _PL101_H

#include <types.h>

void pl101_devicemgr_register_devices();
void pl101_write_string(const uint8_t* c);

#endif