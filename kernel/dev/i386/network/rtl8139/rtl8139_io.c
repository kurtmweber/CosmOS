//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/network/rtl8139/rtl8139_io.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>

void rtl8139_write_dword(struct device *dev, uint16_t offset, uint32_t dword) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata *devicedata = (struct rtl8139_devicedata *)dev->deviceData;
    asm_out_d(devicedata->base + offset, dword);
}

void rtl8139_write_word(struct device *dev, uint16_t offset, uint16_t word) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata *devicedata = (struct rtl8139_devicedata *)dev->deviceData;
    asm_out_w(devicedata->base + offset, word);
}

void rtl8139_write_byte(struct device *dev, uint16_t offset, uint8_t byte) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata *devicedata = (struct rtl8139_devicedata *)dev->deviceData;
    asm_out_b(devicedata->base + offset, byte);
}

uint8_t rtl8139_read_byte(struct device *dev, uint16_t offset) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata *devicedata = (struct rtl8139_devicedata *)dev->deviceData;
    return asm_in_b(devicedata->base + offset);
}
