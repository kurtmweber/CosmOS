//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/RSDT

#ifndef _RSDP_H
#define _RSDP_H

#include <dev/i386/ebda/acpi.h>
#include <types.h>

struct rsdp_descriptor {
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct rsdp_descriptor_2 {
    struct rsdp_descriptor firstPart;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

void rsdp_get_signature(struct rsdp_descriptor_2* rsdp, uint8_t* buffer);
void rsdp_get_oem_id(struct rsdp_descriptor_2* rsdp, uint8_t* buffer);
uint8_t rsdp_get_acpi_version(struct rsdp_descriptor_2* rsdp);

struct acpi_sdt_header* rsdp_get_acpi_header(struct rsdp_descriptor_2* rsdp);
#endif