//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/RTL8139

#include <dev/x86-64/network/rtl8139/rtl8139.h>
#include <dev/x86-64/network/rtl8139/rtl8139_io.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_nic.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>

#define RTL8139_REGISTER_MAC0_5 0x00
#define RTL8139_REGISTER_MAR0_7 0x08
#define RTL8139_REGISTER_RBSTART 0x30
#define RTL8139_REGISTER_CMD 0x37
#define RTL8139_REGISTER_IMR 0x3C
#define RTL8139_REGISTER_ISR 0x3E
#define RTL8139_REGISTER_RCR 0x44
#define RTL8139_REGISTER_CONFIG1 0x52

#define RTL8139_RX_BUFFERSIZE (16 + (1024 * 8) + 1500)

#define RTL8139_DESCRIPTION "Realtek RTL8139 10/100 NIC"

#define RTL8139_RECEIVE 0x01

void rtl8139_clear_interrupt(struct device* dev);
uint16_t rtl8139_get_isr_status(struct device* dev);

void rtl8139_irq_handler_for_device(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*)dev->deviceData;

    kprintf("@\n");
    uint16_t isr_status = rtl8139_get_isr_status(dev);
    if (RTL8139_RECEIVE & isr_status) {
        debug_show_memblock(devicedata->rx_buffer, 64);
        kprintf("\n");
    }
    // TODO check if there is an interrupt set before we clear it!
    rtl8139_clear_interrupt(dev);
}

uint16_t rtl8139_get_isr_status(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    return rtl8139_read_word(dev, RTL8139_REGISTER_ISR);
}

void rtl8139_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    devicemgr_find_devices_by_description(NIC, RTL8139_DESCRIPTION, &rtl8139_irq_handler_for_device);
}

void rtl8139_power_on(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    rtl8139_write_byte(dev, RTL8139_REGISTER_CONFIG1, 0x00);
}

void rtl8139_enable_rx_tx_interrupts(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    // Sets the TOK and ROK bits high
    rtl8139_write_word(dev, RTL8139_REGISTER_IMR, 0x0005);
}

void rtl8139_enable_rx_tx(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    // Sets the RE and TE bits high
    rtl8139_write_byte(dev, RTL8139_REGISTER_CMD, 0x0c);
}

void rtl8139_reset(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    rtl8139_write_byte(dev, RTL8139_REGISTER_CMD, 0x10);
    while ((rtl8139_read_byte(dev, RTL8139_REGISTER_CMD) & 0x10) != 0) {
        sleep_wait(10);
    }
}

void rtl8139_set_rx_buffer(struct device* dev, uint8_t* buffer) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(buffer);

    // 32 bit address
    uint32_t add = LOW_OF_QWORD((uint64_t)buffer);
    rtl8139_write_dword(dev, RTL8139_REGISTER_RBSTART, add);
}

void rtl8139_configure_rcr(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    // accept everything, and do buffer wrap
    rtl8139_write_dword(dev, RTL8139_REGISTER_RCR, 0x0f | 1 << 7);
}

void rtl8139_clear_interrupt(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    rtl8139_write_word(dev, RTL8139_REGISTER_ISR, 0x01);
}

void rtl8139_read_mac(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*)dev->deviceData;

    devicedata->mac[0] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5);
    devicedata->mac[1] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5 + 1);
    devicedata->mac[2] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5 + 2);
    devicedata->mac[3] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5 + 3);
    devicedata->mac[4] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5 + 4);
    devicedata->mac[5] = rtl8139_read_byte(dev, RTL8139_REGISTER_MAC0_5 + 5);
    kprintf("   MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n", devicedata->mac[0], devicedata->mac[1], devicedata->mac[2],
            devicedata->mac[3], devicedata->mac[4], devicedata->mac[5]);
}

/*
 * perform device instance specific init here
 */
uint8_t rtl8139_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*)dev->deviceData;
    devicedata->irq = dev->pci->irq;
    devicedata->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Base %#hX Vendor %#hX Device %#hX (%s)\n", dev->description, devicedata->irq,
            devicedata->base, dev->pci->vendor_id, dev->pci->device_id, dev->name);
    /*
     * mac
     */
    rtl8139_read_mac(dev);
    /*
     * allocate rx buffer
     */
    //  extern void* isadma_buf;
    devicedata->rx_buffer = iobuffers_request_buffer(RTL8139_RX_BUFFERSIZE);
    kprintf("   RX buffer %#hX-%#llX\n", devicedata->rx_buffer, RTL8139_RX_BUFFERSIZE + devicedata->rx_buffer);
    // check that rxbuffer is in lower 2^32 bytes of RAM
    uint64_t rx_buffer_address = (uint64_t)devicedata->rx_buffer;
    memset((uint8_t*)rx_buffer_address, 0, RTL8139_RX_BUFFERSIZE);
    ASSERT((rx_buffer_address & 0xFFFFFFFF) == rx_buffer_address);
    /*
     * register interrupt
     */
    interrupt_router_register_interrupt_handler(devicedata->irq, &rtl8139_irq_handler);
    /*
     * power on
     */
    rtl8139_power_on(dev);
    /*
     * reset
     */
    rtl8139_reset(dev);
    /*
     * rx buffer
     */
    rtl8139_set_rx_buffer(dev, devicedata->rx_buffer);
    /*
     * interrupts
     */
    rtl8139_enable_rx_tx_interrupts(dev);
    /*
     * config rcr
     */
    rtl8139_configure_rcr(dev);
    /*
     * enable rx tx
     */
    rtl8139_enable_rx_tx(dev);
    return 1;
}

void rtl8139_ethernet_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);

    //   struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*)dev->deviceData;

    panic("Ethernet read not implemented yet");
}

void rtl8139_ethernet_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*)dev->deviceData;
    // i dunno, some magic
    ASSERT(size < 1792);

    /*
     * register pair
     */
    uint8_t txstart;
    uint8_t txstatus;
    rtl8139_get_registers(devicedata->write_count, &txstart, &txstatus);
    kprintf("write register pair %llu $%llu\n", txstart, txstatus);
    /*
     * TODO this needs to be a PHYSICAL address!
     */
    /*
     * 32-bit address of buffer
     */
    // FIX this
    //  rtl8139_write_dword(dev, txstart, (uint32_t)data);
    /*
     * status
     */
    uint16_t status = size;  // lower 12 bits
    /*
     * set bit 13 to tell the controller to start transmitting
     */
    status = status | (1 << 13);
    /*
     * set status
     */
    rtl8139_write_word(dev, txstatus, status);
    /*
     * wait for bit 15 to be set
     */
    uint32_t stat = rtl8139_read_word(dev, txstatus);

    while (0 == (stat & (1 << 15))) {
        sleep_wait(10);
        stat = rtl8139_read_word(dev, txstatus);
    }
}

void rtl8139_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &rtl8139_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = NIC;
    devicemgr_set_device_description(deviceinstance, RTL8139_DESCRIPTION);
    /*
     * the device api
     */
    struct deviceapi_nic* api = (struct deviceapi_nic*)kmalloc(sizeof(struct deviceapi_nic));
    api->write = &rtl8139_ethernet_read;
    api->read = &rtl8139_ethernet_write;
    deviceinstance->api = api;
    /*
     * the deviceData
     */
    struct rtl8139_devicedata* deviceData = (struct rtl8139_devicedata*)kmalloc(sizeof(struct rtl8139_devicedata));
    deviceData->base = 0;
    deviceData->irq = 0;
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all RTL8139 devices and register them
 */
void rtl8139_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, 0x10EC, 0x8139, &rtl8139_search_cb);
}
