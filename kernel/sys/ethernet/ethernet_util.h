//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2021 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_UTIL_H
#define _ETHERNET_UTIL_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

void ethernet_read(struct device* dev, uint8_t* data, uint32_t size);
void ethernet_write(struct device* dev, uint8_t* data, uint32_t size);

#endif
