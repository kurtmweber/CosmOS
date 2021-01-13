//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _COM1_H
#define _COM1_H

#include <types.h>

/**
 * this special function is used at boot time to write only to com1 for console output
 */
void com1_serial_write(const uint8_t* c);
void com1_serial_write_char(const uint8_t c);

#endif