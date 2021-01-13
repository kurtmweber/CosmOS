//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/i386/network/rtl8139/rtl8139.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/asm/asm.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/console/console.h>
#include <dev/i386/pci/pci.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/debug/assert.h>
#include <sys/i386/mm/mm.h>
#include <sys/sleep/sleep.h>
#include <sys/console/console.h>

#define RTL8139_REGISTER_MAC0_5     0x00
#define RTL8139_REGISTER_MAR0_7     0x08
#define RTL8139_REGISTER_RBSTART    0x30
#define RTL8139_REGISTER_CMD        0x37
#define RTL8139_REGISTER_IMR        0x3C
#define RTL8139_REGISTER_ISR        0x3E
#define RTL8139_REGISTER_RCR        0x44
#define RTL8139_REGISTER_CONFIG1    0x52

#define RTL8139_RX_BUFFERSIZE       (16+(1024*8)+1500)

struct rtl8139_devicedata {
    uint64_t base;
    uint16_t irq;
    uint8_t* rx_buffer;
} __attribute__((packed));

void rtl8139_irq_handler(stackFrame *frame){
	ASSERT_NOT_NULL(frame);
    kprintf("@");
}

void rtl8139_write_dword(struct device* dev, uint16_t offset, uint32_t dword) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*) dev->deviceData;
    asm_out_d(devicedata->base+offset, dword);
}

void rtl8139_write_word(struct device* dev, uint16_t offset, uint16_t word) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*) dev->deviceData;
    asm_out_w(devicedata->base+offset, word);
}

void rtl8139_write_byte(struct device* dev, uint16_t offset, uint8_t byte) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*) dev->deviceData;
    asm_out_b(devicedata->base+offset, byte);
}

uint8_t rtl8139_read_byte(struct device* dev, uint16_t offset) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*) dev->deviceData;
    return asm_in_b(devicedata->base+offset);
}

void rtl8139_power_on(struct device* dev){
	ASSERT_NOT_NULL(dev);
    rtl8139_write_byte(dev, RTL8139_REGISTER_CONFIG1, 0x00);
}

void rtl8139_enable_rx_tx_interrupts(struct device* dev){
	ASSERT_NOT_NULL(dev);
    // Sets the TOK and ROK bits high
    rtl8139_write_word(dev, RTL8139_REGISTER_IMR, 0x0005);
}

void rtl8139_enable_rx_tx(struct device* dev){
	ASSERT_NOT_NULL(dev);
    // Sets the RE and TE bits high
    rtl8139_write_byte(dev, RTL8139_REGISTER_CMD, 0x0c);
}

void rtl8139_reset(struct device* dev){
	ASSERT_NOT_NULL(dev);
    rtl8139_write_byte(dev, RTL8139_REGISTER_CMD, 0x10);
    while((rtl8139_read_byte(dev,RTL8139_REGISTER_CMD) & 0x10)!=0){
        sleep_wait(10);
    }
}

void rtl8139_set_rx_buffer(struct device* dev, uint8_t* buffer){
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(buffer);
    rtl8139_write_dword(dev, RTL8139_REGISTER_RBSTART,(uint32_t)* buffer);
}

void rtl8139_configure_rcr(struct device* dev){
	ASSERT_NOT_NULL(dev);
    // accept everything, and do buffer wrap
    rtl8139_write_dword(dev, RTL8139_REGISTER_RCR, 0x0f | 1 << 7);
}

void rtl8139_clear_interrupt(struct device* dev){
	ASSERT_NOT_NULL(dev);
    rtl8139_write_word(dev, RTL8139_REGISTER_ISR, 0x01);
}

/*
* perform device instance specific init here
*/
void rtl8139_init(struct device* dev){
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(dev->deviceData);
    struct rtl8139_devicedata* devicedata = (struct rtl8139_devicedata*) dev->deviceData;
    devicedata->irq = dev->pci->irq;
    devicedata->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Base %#hX Vendor %#hX Device %#hX (%s)\n",dev->description, devicedata->irq ,devicedata->base, dev->pci->vendor_id, dev->pci->device_id, dev->name);
    /*
    * allocate rx buffer
    */
    devicedata->rx_buffer = kmalloc(RTL8139_RX_BUFFERSIZE);
    /*
    * register interrupt
    */
    interrupt_router_register_interrupt_handler(devicedata->irq , &rtl8139_irq_handler);
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
}

void rtl8139_ethernet_read(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(data);

	panic("Ethernet read not implemented yet");
}
void rtl8139_ethernet_write(struct device* dev, uint8_t* data, uint32_t size) {
	ASSERT_NOT_NULL(dev);
	ASSERT_NOT_NULL(data);

	panic("Ethernet write not implemented yet");
}

void rtl8139_search_cb(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &rtl8139_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = NIC;
    devicemgr_set_device_description(deviceinstance, "Realtek RTL8139 10/100 NIC");
    /*
    * the device api
    */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*) kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &rtl8139_ethernet_read;
    api->read = &rtl8139_ethernet_write;
    deviceinstance->api = api;
    /*
    * the deviceData
    */
    struct rtl8139_devicedata* deviceData = (struct rtl8139_devicedata*) kmalloc(sizeof(struct rtl8139_devicedata));
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
    pci_devicemgr_search_device(PCI_CLASS_NETWORK,PCI_NETWORK_SUBCLASS_ETHERNET,0x10EC,0x8139, &rtl8139_search_cb);
}
