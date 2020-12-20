//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _MOUSE_H
#define _MOUSE_H

#include <types.h>

void mouse_devicemgr_register_devices();

extern struct mouse_status* current_mouse_status;

#endif