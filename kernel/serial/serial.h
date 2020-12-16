//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIAL_H
#define _SERIAL_H

#include <types.h>

void serial_init();
void serial_write(uint8_t* c);

#endif