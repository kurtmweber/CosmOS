/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/i386/ata/ata.h>
#include <dev/i386/ata/ata_controller.h>
#include <dev/i386/ata/ata_disk.h>
#include <dev/i386/ata/ata_identity.h>
#include <dev/i386/ata/ata_util.h>
#include <dev/i386/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/string.h>
#include <types.h>

void ata_detect_devices(struct device* device, struct ata_controller* controller);
#define IDE_SERIAL_IRQ 14

/*
 * detect all the addresses on a ATA controller
 */
void ata_detect_addresses(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT_NOT_NULL(dev->pci);

    struct ata_controller* controller = (struct ata_controller*)dev->deviceData;
    uint8_t bus = dev->pci->bus;
    uint8_t device = dev->pci->device;
    uint8_t function = dev->pci->function;

    uint32_t bar_result;

    bar_result = pci_header_read_bar0(bus, device, function);
    controller->channels[ATA_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x1F0 : bar_result);

    bar_result = pci_header_read_bar1(bus, device, function);
    controller->channels[ATA_PRIMARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x3F6 : bar_result);

    bar_result = pci_header_read_bar2(bus, device, function);
    controller->channels[ATA_SECONDARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x170 : bar_result);

    bar_result = pci_header_read_bar3(bus, device, function);
    controller->channels[ATA_SECONDARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x376 : bar_result);

    bar_result = pci_header_read_bar4(bus, device, function);
    //  kprintf("bar4 %#llX\n", bar_result);
    controller->channels[ATA_PRIMARY].dma_bus_master_register = bar_result;
    controller->channels[ATA_SECONDARY].dma_bus_master_register = bar_result + 0x08;
}

/*
 * init ATA controller
 */
uint8_t device_init_ata(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    struct ata_controller* controller = (struct ata_controller*)dev->deviceData;

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id,
            dev->pci->device_id, dev->name);

    // set up selected devices
    controller->channels[ATA_PRIMARY].selected_device = ATA_DRIVE_SELECT_NONE;
    controller->channels[ATA_SECONDARY].selected_device = ATA_DRIVE_SELECT_NONE;

    // detect addresses
    ata_detect_addresses(dev);

    kprintf("    Primary IDE I/O at %#llX, control at %#llX\n", controller->channels[ATA_PRIMARY].base_io,
            controller->channels[ATA_PRIMARY].base_io_ctrl);
    kprintf("    Secondary IDE I/O at %#llX, control at %#llX\n", controller->channels[ATA_SECONDARY].base_io,
            controller->channels[ATA_SECONDARY].base_io_ctrl);

    // if this doesn't set the IRQ then this is a parallel IDE, but we don't need to know that
    pci_header_set_irq(dev->pci->bus, dev->pci->device, dev->pci->function, IDE_SERIAL_IRQ);

    // turn off interrupts
    ata_interrupt_enable(controller, ATA_PRIMARY, false);
    ata_interrupt_enable(controller, ATA_SECONDARY, false);

    ata_detect_devices(dev, controller);

    return 1;
}

void ata_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &device_init_ata;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = ATA;
    devicemgr_set_device_description(deviceinstance, "ATA");
    /*
     * device data
     */
    struct ata_controller* deviceData = (struct ata_controller*)kmalloc(sizeof(struct ata_controller));
    deviceinstance->deviceData = deviceData;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

void ata_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_MASS_STORAGE, PCI_MASS_STORAGE_SUBCLASS_IDE, &ata_search_cb);
}

void ata_detect_devices(struct device* device, struct ata_controller* controller) {
    uint8_t i, j;
    uint8_t status;
    struct ata_device_t* tmp;

    for (i = 0; i < 2; i++) {
        controller->channels[i].selected_device = ATA_DRIVE_SELECT_NONE;
        for (j = 0; j < 2; j++) {
            ata_select_device(controller, i, j);

            ata_register_write(controller, i, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
            sleep_wait(1);

            status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
            if (!status) {  // no device found
                controller->channels[i].devices[j].exists = false;
                continue;
            }

            // set the exists flag to true
            controller->channels[i].devices[j].exists = true;

            // status isn't 0, so the device exists--so we poll until ready
            while (1) {
                status = ata_register_read(controller, i, ATA_REGISTER_STATUS);

                if (status & ATA_STATUS_ERROR) {
                    // do ATAPI identify, it requires a packet request
                    ata_detect_atapi(controller, i);
                    break;
                } else if (!(status & ATA_STATUS_BUSY) && (status & ATA_STATUS_DRQ)) {
                    break;
                }
            }

            controller->channels[i].devices[j].identity = ata_detect_read_identify(controller, i);
            ata_extract_identity(controller->channels[i].devices[j].identity, &(controller->channels[i].devices[j]));

            // register the device
            //	kprintf("    Found disk at channel %llu, device %llu\n",i,j);
            ata_register_disk(device, i, j);
        }
    }
    return;
}

struct ata_device* ata_get_disk(struct device* dev, uint8_t channel, uint8_t disk) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->deviceData);
    ASSERT(((channel >= 0) && (channel <= 1)));
    ASSERT(((disk >= 0) && (disk <= 1)));
    struct ata_controller* controller = (struct ata_controller*)dev->deviceData;
    return &(controller->channels[channel].devices[disk]);
}
