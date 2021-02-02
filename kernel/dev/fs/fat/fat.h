//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FAT_H
#define _FAT_H

#include <types.h>

#define FAT_MBR_PARTITION_ID "B"
#define FAT_GPT_PARTITION_ID "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7"

struct device* fat_attach(struct device* partition_device);
void fat_detach(struct device* dev);

#endif