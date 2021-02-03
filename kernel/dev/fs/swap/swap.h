//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SWAP_H
#define _SWAP_H

#include <types.h>

struct device;

struct device* swap_attach(struct device* block_device);
void swap_detach(struct device* dev);

#endif
