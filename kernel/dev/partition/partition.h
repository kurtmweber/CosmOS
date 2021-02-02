//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* note that partitions implement the deviceapi_block API.
*/
#ifndef _PARTITION_H
#define _PARTITION_H

#include <types.h>

struct device;

struct device* partition_attach(struct device* partition_table_device, uint8_t partition_index);
void partition_detach(struct device* dev);

#endif