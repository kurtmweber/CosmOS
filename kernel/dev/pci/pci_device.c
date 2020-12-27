//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/pci/pci_device.h>
#include <types.h>
#include <console/console.h>
#include <panic/panic.h>

struct list *pci_devices;

void pci_devicemgr_search_device(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id, uint16_t device_id, pcideviceSearchCallback cb){
    ASSERT_NOT_NULL(pci_devices, "pci_devices must not be null.  Did you init PCI?");
    uint16_t i = 0;
    for (i = 0; i < list_count(pci_devices); i++){
        struct pci_device* dev = (struct pci_device*) list_get(pci_devices,i);
        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            if ((dev->vendor_id==vendor_id) && (dev->device_id==device_id)) {
                (*cb)(dev);
            }
        }
    }
}

void pci_devicemgr_search_devicetype(pci_class_codes pci_class, uint8_t pci_subclass, pcideviceSearchCallback cb){
    ASSERT_NOT_NULL(pci_devices, "pci_devices must not be null.  Did you init PCI?");
    uint16_t i = 0;
    for (i = 0; i < list_count(pci_devices); i++){
        struct pci_device* dev = (struct pci_device*) list_get(pci_devices,i);
        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            (*cb)(dev);            
        }
    }
}
