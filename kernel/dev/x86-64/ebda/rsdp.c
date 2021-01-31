//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/ebda/rsdp.h>
#include <sys/asm/byte.h>
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
    // not implemented
    ASSERT(0);
    // this returns a PHYSICAL address, which happens to not be in
    // the lower 1MB which is identity mapped.  Therefore we need to
    // map it into our virtual memory space
    //    kprintf("RADT %#llX\n", rsdp->firstPart.rsdt_address);
    return (struct acpi_sdt_header*)(uint64_t)rsdp->firstPart.rsdt_address;
}

uint8_t rsdp_is_valid(struct rsdp_descriptor_2* rsdp) {
    uint32_t sum = 0;
    uint8_t* fp = (uint8_t*)&(rsdp->firstPart);
    for (uint16_t i = 0; i < sizeof(rsdp->firstPart); i++) {
        sum = sum + fp[i];
    }
    ASSERT(0 == LOW_OF_WORD(sum));
    return (0 == LOW_OF_WORD(sum));
}
