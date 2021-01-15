//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
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
uint8_t bda_num_hd();
uint16_t bda_video_port();
uint16_t bda_uptime();
uint8_t bda_display_mode();
uint16_t bda_text_columns();
uint64_t bda_get_ebda_address();
#endif