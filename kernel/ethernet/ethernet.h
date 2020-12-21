//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <types.h>


void send(uint8_t* destinationMAC, uint8_t* packet);
uint8_t* receive();

#endif

