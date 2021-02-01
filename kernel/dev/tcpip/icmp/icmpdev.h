//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ICMPDEV_H
#define _ICMPDEV_H

#include <types.h>

struct device;

struct device* icmp_attach(struct device* ethernet_device);
void icmp_detach(struct device* dev);

#endif