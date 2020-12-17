//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <pci/pci_device.h>
#include <types.h>
#include <console/console.h>

struct pci_device *pci_devices;
uint16_t num_pci_devices;

void pci_search(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id, uint16_t device_id, deviceSearchCallback cb){
    uint16_t i = 0;
    for (i = 0; i < num_pci_devices; i++){
        if ((pci_devices[i].pci_class == pci_class) && (pci_devices[i].pci_subclass == pci_subclass)) {
            if ((pci_devices[i].vendor_id==vendor_id) && (pci_devices[i].device_id==device_id)) {
                kprintf("PCI Device found at PCI address %#hX:%#hX:%#hX\n", pci_devices[i].bus, pci_devices[i].device, pci_devices[i].function);
                kprintf("\tVendor %#X, Device %#X\n", pci_devices[i].vendor_id, pci_devices[i].device_id);
                (*cb)(&(pci_devices[i]));
            }
        }
    }
}
