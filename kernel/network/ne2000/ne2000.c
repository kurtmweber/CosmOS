//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <network/ne2000/ne2000.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <device/device.h>
#include <console/console.h>
#include <pci/pci.h>

void ne2000_irq_handler(){
}
/*
* perform device instance specific init here
*/
void deviceInitNE200(struct device* dev){
    struct pci_device_t* pci_dev = (struct pci_device_t*) dev->deviceData;
    registerInterruptHandler(pci_dev->irq, &ne2000_irq_handler);
    kprintf("Init NE2000 at IRQ %llu\n", pci_dev->irq);
}

/**
* find all NE2000 devices and register them
*/
void ne2000_register_devices() {
    uint16_t i = 0;
    for (i = 0; i < num_pci_devices; i++){
        if ((pci_devices[i].pci_class == PCI_CLASS_NETWORK) && ((pci_network_subclass_codes)pci_devices[i].pci_subclass == PCI_NETWORK_SUBCLASS_ETHERNET)) {
            if ((pci_devices[i].vendor_id==0x10EC) && (pci_devices[i].device_id==0x8029)) {
                kprintf("NE2000 Network Controller found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
                kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
                /*
                * register device
                */
                struct device* deviceinstance = newDevice();
                deviceinstance->init =  &deviceInitNE200;
                deviceinstance->deviceData = &(pci_devices[i]);
                registerDevice(deviceinstance);
            }
        }
    }
}
