//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ISADMA_PAGE_H
#define _ISADMA_PAGE_H

#include <types.h>

uint16_t isadma_address_to_page(uint64_t address);
uint16_t isadma_address_to_buffer(uint64_t address);

#endif