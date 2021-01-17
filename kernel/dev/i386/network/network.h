//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NETWORK_H
#define _NETWORK_H

#include <dev/i386/network/e1000/e1000.h>
#include <dev/i386/network/ne2000isa/ne2000isa.h>
#include <dev/i386/network/ne2000pci/ne2000pci.h>
#include <dev/i386/network/rtl8139/rtl8139.h>
#include <dev/virtio/vnic/vnic.h>

void network_devicemgr_register_devices();

#endif
