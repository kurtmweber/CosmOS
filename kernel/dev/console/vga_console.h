//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VGA_CONSOLE_H
#define _VGA_CONSOLE_H

struct device;

struct device* vga_console_attach(struct device* vga_device);
void vga_console_detach(struct device* dev);

#endif
