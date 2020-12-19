//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <pci/pci_device.h>
#include <types.h>
#include <console/console.h>

struct list *pci_devices;

void pci_search_device(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id, uint16_t device_id, pcideviceSearchCallback cb){
    uint16_t i = 0;
    for (i = 0; i < listCount(pci_devices); i++){
        struct pci_device* dev = (struct pci_device*) listGet(pci_devices,i);
        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            if ((dev->vendor_id==vendor_id) && (dev->device_id==device_id)) {
                (*cb)(dev);
            }
        }
    }
}

void pci_search_devicetype(pci_class_codes pci_class, uint8_t pci_subclass, pcideviceSearchCallback cb){
    uint16_t i = 0;
    for (i = 0; i < listCount(pci_devices); i++){
        struct pci_device* dev = (struct pci_device*) listGet(pci_devices,i);
        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            (*cb)(dev);            
        }
    }
}