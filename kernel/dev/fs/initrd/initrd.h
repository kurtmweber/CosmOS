//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html
*/
#ifndef _INITRD_H
#define _INITRD_H

#include <types.h>

struct device* initrd_attach(struct device* partition_device);
void initrd_detach(struct device* dev);

#endif