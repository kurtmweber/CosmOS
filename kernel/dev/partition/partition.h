//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PARTITION_H
#define _PARTITION_H

#include <types.h>

struct device;
struct device;

struct device* partition_attach(struct device* block_device, uint64_t lba);
void partition_detach(struct device* dev);

#endif