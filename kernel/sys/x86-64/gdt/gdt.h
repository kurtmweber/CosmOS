//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Getting_to_Ring_3

#ifndef _GDT_H
#define _GDT_H

#include <types.h>

void gdt_install();
void extern gdt_flush();

#endif