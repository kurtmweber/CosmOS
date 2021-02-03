//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FS_UTIL_H
#define _FS_UTIL_H

struct device;

void fsutil_attach_partition_tables(struct device* block_dev);
void fsutil_detach_partition_tables(struct device* block_dev);

void fsutil_attach_partitions(struct device* partition_table_dev);
void fsutil_detach_partitions(struct device* partition_table_dev);

void fsutil_attach_fs(struct device* partition_dev);
void fsutil_detach_fs(struct device* partition_dev);

#endif