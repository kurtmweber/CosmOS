//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/network/rtl8139/rtl8139_io.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>

#define RTL8139_REGISTER_TX_START_0 0x20
#define RTL8139_REGISTER_TX_START_1 0x24
#define RTL8139_REGISTER_TX_START_2 0x28
#define RTL8139_REGISTER_TX_START_3 0x2c
#define RTL8139_REGISTER_TX_STATUS_0 0x10
#define RTL8139_REGISTER_TX_STATUS_1 0x14
#define RTL8139_REGISTER_TX_STATUS_2 0x18
#define RTL8139_REGISTER_TX_STATUS_3 0x1c

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

uint16_t rtl8139_read_word(struct device *dev, uint16_t offset) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata *devicedata = (struct rtl8139_devicedata *)dev->deviceData;
    return asm_in_w(devicedata->base + offset);
}

void rtl8139_get_registers(uint64_t write_count, uint8_t *txstart, uint8_t *txstatus) {
    uint8_t idx = write_count % 4;
    switch (idx) {
        case 0:
            *txstart = RTL8139_REGISTER_TX_START_0;
            *txstatus = RTL8139_REGISTER_TX_STATUS_0;
            return;
        case 1:
            *txstart = RTL8139_REGISTER_TX_START_1;
            *txstatus = RTL8139_REGISTER_TX_STATUS_1;
            return;
        case 2:
            *txstart = RTL8139_REGISTER_TX_START_2;
            *txstatus = RTL8139_REGISTER_TX_STATUS_2;
            return;
        case 3:
            *txstart = RTL8139_REGISTER_TX_START_3;
            *txstatus = RTL8139_REGISTER_TX_STATUS_3;
            return;
        default:
            panic("Illegal offset\n");
    }
}
