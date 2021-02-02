//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CPM_H
#define _CPM_H

#include <types.h>

struct device* cpm_attach(struct device* partition_device);
void cpm_detach(struct device* dev);

#endif