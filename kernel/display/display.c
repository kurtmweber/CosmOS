//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <display/display.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <pci/pci.h>
#include <console/console.h>
#include <device/device.h>

/*
* perform device instance specific init here
*/
void deviceInitDisplay(struct device* dev){
    struct pci_device_t* pci_dev = (struct pci_device_t*) dev->deviceData;
    kprintf("Init Display at IRQ %llu\n", pci_dev->irq);
}

/**
* find all Display devices and register them
*/
void display_register_devices() {
    uint16_t i = 0;
    for (i = 0; i < num_pci_devices; i++){
        if ((pci_devices[i].pci_class == PCI_CLASS_DISPLAY) && ((pci_display_subclass_codes)pci_devices[i].pci_subclass == PCI_DISPLAY_SUBCLASS_VGA)) {
			kprintf("VGA Controller found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
			kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
       
            /*
            * register device
            */
            struct device* deviceinstance = newDevice();
            deviceinstance->init =  &deviceInitDisplay;
            deviceinstance->deviceData = &(pci_devices[i]);
            registerDevice(deviceinstance);
        }
    }
}
