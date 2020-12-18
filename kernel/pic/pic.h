//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PIC_H
#define _PIC_H

#include <types.h>

void pic_register_devices();
void pic_send_eoi(uint8_t irq);
void pic_init();

#endif