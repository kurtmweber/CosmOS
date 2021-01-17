//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/GPT

#include <dev/fs/block_util.h>
#include <dev/fs/guid_pt.h>
#include <sys/debug/assert.h>

#define GUID_PT_HEADER_LBA 1

const uint8_t GUID_PT_EFI_PART[] = {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};

void guid_pt_read_guid_pt_header(struct device* dev, struct guid_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);

    block_read(dev, GUID_PT_HEADER_LBA, (uint8_t*)header, sizeof(struct guid_pt_header));
    ASSERT(header->magic[0] == GUID_PT_EFI_PART[0]);
}