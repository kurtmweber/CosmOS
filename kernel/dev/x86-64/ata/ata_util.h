/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_UTIL_H
#define _ATA_UTIL_H

#include <dev/x86-64/ata/ata.h>
#include <types.h>

struct ata_controller;

uint8_t ata_register_read(struct ata_controller* controller, uint8_t channel, ata_registers reg);
uint32_t ata_register_read_dword(struct ata_controller* controller, uint8_t channel, ata_registers reg);
uint16_t ata_register_read_word(struct ata_controller* controller, uint8_t channel, ata_registers reg);
void ata_register_write(struct ata_controller* controller, uint8_t channel, ata_registers reg, uint8_t value);
void ata_register_write_word(struct ata_controller* controller, uint8_t channel, ata_registers reg, uint16_t value);
uint16_t ata_register_port_number(struct ata_controller* controller, uint8_t channel, ata_registers reg);
void ata_detect_atapi(struct ata_controller* controller, uint8_t channel);
bool ata_channel_ready(struct ata_controller* controller, uint8_t channel);
bool ata_select_device(struct ata_controller* controller, uint8_t channel, uint8_t device);
void ata_interrupt_enable(struct ata_controller* controller, uint8_t channel, bool enabled);
void ata_wait_busy(struct ata_controller* controller, uint8_t channel);
void ata_wait_drq(struct ata_controller* controller, uint8_t channel);

#endif