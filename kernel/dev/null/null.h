//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NULL_H
#define _NULL_H

#include <types.h>

struct device;

struct device* null_attach();
void null_detach(struct device* dev);

#endif