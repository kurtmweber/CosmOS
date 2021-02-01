//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _UDPDEV_H
#define _UDPDEV_H

#include <types.h>

struct device;

struct device* udp_attach(struct device* ip_device);
void udp_detach(struct device* dev);

#endif