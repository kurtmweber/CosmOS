//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <types.h>

struct device;

struct device* ethernet_attach(struct device* ethernet_device);
void ethernet_detach(struct device* dev);

#endif