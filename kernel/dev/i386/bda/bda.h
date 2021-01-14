//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BDA_H
#define _BDA_H

#include <types.h>

void bda_devicemgr_register_devices();

uint16_t bda_serial0_base();
uint16_t bda_serial1_base();
uint16_t bda_serial2_base();
uint16_t bda_serial3_base();
uint16_t bda_parallel0_base();
uint16_t bda_parallel1_base();
uint16_t bda_parallel2_base();

#endif