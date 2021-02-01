//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _IPDEV_H
#define _IPDEV_H

#include <types.h>

struct device;

struct device* ip_attach(struct device* ethernet_device);
void ip_detach(struct device* dev);

#endif