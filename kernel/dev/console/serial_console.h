//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIAL_CONSOLE_H
#define _SERIAL_CONSOLE_H

struct device;

struct device* serial_console_attach(struct device* serial_device);
void serial_console_detach(struct device* dev);

#endif
