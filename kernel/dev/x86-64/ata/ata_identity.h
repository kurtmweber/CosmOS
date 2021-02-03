/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_IDENTITY_H
#define _ATA_IDENTITY_H

#include <dev/x86-64/ata/ata.h>
#include <dev/x86-64/ata/ata_controller.h>
#include <types.h>

struct ata_controller;

uint32_t ata_detect_extract_dword(const char* identify_buf, ata_identify_offsets offset);
uint64_t ata_detect_extract_qword(const char* identify_buf, ata_identify_offsets offset);
uint16_t ata_detect_extract_word(const char* identify_buf, ata_identify_offsets offset);
char* ata_detect_extract_string(const char* identify_buf, uint8_t len, ata_identify_offsets offset);
uint32_t ata_detect_sector_size(const char* identify_buf);
char* ata_detect_read_identify(struct ata_controller* controller, uint8_t channel);

void ata_extract_identity(const char* identity, struct ata_device* dev);

#endif