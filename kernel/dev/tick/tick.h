//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TICK_H
#define _TICK_H

#include <types.h>

struct device;

struct device* tick_attach(struct device* pit_device);
void tick_detach(struct device* dev);

#endif