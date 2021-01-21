//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/ebda/rsdp.h>
#include <sys/debug/assert.h>
#include <sys/string/mem.h>

void rsdp_get_signature(struct rsdp_descriptor_2* rsdp, uint8_t* buffer) {
    ASSERT_NOT_NULL(rsdp);
    ASSERT_NOT_NULL(buffer);
    memcpy(buffer, rsdp->firstPart.signature, 8);
    buffer[8] = 0;
}

void rsdp_get_oem_id(struct rsdp_descriptor_2* rsdp, uint8_t* buffer) {
    ASSERT_NOT_NULL(rsdp);
    ASSERT_NOT_NULL(buffer);
    memcpy(buffer, rsdp->firstPart.oem_id, 6);
    buffer[6] = 0;
}

uint8_t rsdp_get_acpi_version(struct rsdp_descriptor_2* rsdp) {
    ASSERT_NOT_NULL(rsdp);
    return rsdp->firstPart.revision;
}

struct acpi_sdt_header* rsdp_get_acpi_header(struct rsdp_descriptor_2* rsdp) {
    ASSERT_NOT_NULL(rsdp);
    //    kprintf("RADT %#llX\n", rsdp->firstPart.rsdt_address);
    return (struct acpi_sdt_header*)(uint64_t)rsdp->firstPart.rsdt_address;
}
