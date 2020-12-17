//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <network/rtl8139/rtl8139.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <device/device.h>
#include <console/console.h>
#include <pci/pci.h>

void rtl8139_irq_handler(){

}

/*
* perform device instance specific init here
*/
void deviceInitRTL8139(struct device* dev){
    struct pci_device_t* pci_dev = (struct pci_device_t*) dev->deviceData;
    registerInterruptHandler(pci_dev->irq, &rtl8139_irq_handler);
    kprintf("Init RTL8139 at IRQ %llu\n", pci_dev->irq);
}

/**
* find all RTL8139 devices and register them
*/
void rtl8139_register_devices() {
    uint16_t i = 0;
    for (i = 0; i < num_pci_devices; i++){
        if ((pci_devices[i].pci_class == PCI_CLASS_NETWORK) && ((pci_network_subclass_codes)pci_devices[i].pci_subclass == PCI_NETWORK_SUBCLASS_ETHERNET)) {
            if ((pci_devices[i].vendor_id==0x10EC) && (pci_devices[i].device_id==0x8139)) {
                kprintf("RTL 8139 Network Controller found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
                kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
                /*
                * register device
                */
                struct device* deviceinstance = newDevice();
                deviceinstance->init =  &deviceInitRTL8139;
                deviceinstance->deviceData = &(pci_devices[i]);
                registerDevice(deviceinstance);
            }
        }
    }
}
