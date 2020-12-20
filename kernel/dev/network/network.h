//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NETWORK_H
#define _NETWORK_H

#include <dev/network/ne2000pci/ne2000pci.h>
#include <dev/network/ne2000isa/ne2000isa.h>
#include <dev/network/rtl8139/rtl8139.h>
#include <dev/network/e1000/e1000.h>

void network_devicemgr_register_devices();

#endif

