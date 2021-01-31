//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RAND_H
#define _RAND_H

#include <types.h>

struct device;

struct device* rand_attach();
void rand_detach(struct device* dev);

#endif