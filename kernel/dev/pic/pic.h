//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PIC_H
#define _PIC_H

#include <types.h>

// IRQ vector offsets for remapping
#define PIC_PRIMARY_VECTOR_OFFSET	0x20
#define PIC_SECONDARY_VECTOR_OFFSET	PIC_PRIMARY_VECTOR_OFFSET + 8

void pic_register_devices();
void pic_send_eoi(uint8_t irq);
void pic_register_devices();

#endif