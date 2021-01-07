/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/


#ifndef _ATA_UTIL_H
#define _ATA_UTIL_H

#include <types.h>
#include <dev/i386/ata/ata.h>

struct ata_controller;

uint32_t ata_detect_extract_dword(char *identify_buf, ata_identify_offsets offset);
uint64_t ata_detect_extract_qword(char *identify_buf, ata_identify_offsets offset);
uint8_t ata_register_read(struct ata_controller* controller, uint8_t channel, ata_registers reg);
uint32_t ata_register_read_dword(struct ata_controller* controller, uint8_t channel, ata_registers reg);
uint16_t ata_register_read_word(struct ata_controller* controller, uint8_t channel, ata_registers reg);
void ata_register_write(struct ata_controller* controller, uint8_t channel, ata_registers reg, uint8_t value);
uint16_t ata_detect_extract_word(char *identify_buf, ata_identify_offsets offset);
char *ata_detect_extract_string(char *identify_buf, uint8_t len, ata_identify_offsets offset);
uint16_t ata_register_port_number(struct ata_controller* controller, uint8_t channel, ata_registers reg);
uint32_t ata_detect_sector_size(char *identify_buf);
char *ata_detect_read_identify(struct ata_controller* controller, uint8_t channel);
void ata_detect_atapi(struct ata_controller* controller, uint8_t channel);
bool ata_channel_ready(struct ata_controller* controller, uint8_t channel);

#endif