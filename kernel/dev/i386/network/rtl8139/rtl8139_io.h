//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _RTL8139_IO_H
#define _RTL8139_IO_H

#include <sys/devicemgr/devicemgr.h>
#include <types.h>

struct rtl8139_devicedata {
    uint8_t mac[6];
    uint64_t base;
    uint16_t irq;
    uint8_t *rx_buffer;
} __attribute__((packed));

void rtl8139_write_dword(struct device *dev, uint16_t offset, uint32_t dword);
void rtl8139_write_word(struct device *dev, uint16_t offset, uint16_t word);
void rtl8139_write_byte(struct device *dev, uint16_t offset, uint8_t byte);
uint8_t rtl8139_read_byte(struct device *dev, uint16_t offset);
#endif