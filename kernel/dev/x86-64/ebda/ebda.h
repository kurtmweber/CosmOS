//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _EBDA_H
#define _EBDA_H

#include <dev/x86-64/ebda/rsdp.h>
#include <types.h>

void ebda_devicemgr_register_devices();

struct rsdp_descriptor_2* ebda_get_rsdp();

#endif