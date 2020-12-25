//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/usb/usb.h>
#include <interrupts/interrupt_router.h>
#include <asm/asm.h>
#include <dev/pci/pci.h>
#include <console/console.h>
#include <devicemgr/devicemgr.h>

#define USB_EHCI_CAPABILITY_REGISTER        0x00
#define USB_EHCI_HCIVERSION_REGISTER        0x02
#define USB_EHCI_HCSPARAMS_REGISTER         0x04
#define USB_EHCI_HCCPARAMS_REGISTER         0x08
#define USB_EHCI_HCSP_PORTROUTE_REGISTER    0x0C

#define USB_EHCI_USBCMD_OPERATIONAL_REGISTER            0x00
#define USB_EHCI_USBSTS_OPERATIONAL_REGISTER            0x04
#define USB_EHCI_USBINTR_OPERATIONAL_REGISTER           0x08
#define USB_EHCI_FRINDEX_OPERATIONAL_REGISTER           0x0C

#define USB_EHCI_CTRLDSSEGMENT_OPERATIONAL_REGISTER     0x10
#define USB_EHCI_PERIODICLISTBASE_OPERATIONAL_REGISTER  0x14
#define USB_EHCI_ASYNCLISTADDR_OPERATIONAL_REGISTER     0x18

#define USB_EHCI_CONFIGFLAG_OPERATIONAL_REGISTER        0x40
#define USB_EHCI_PORTSC_OPERATIONAL_REGISTER            0x44

/*
* perform device instance specific init here
*/
void deviceInitUSB(struct device* dev){
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n",dev->description, dev->pci->irq,dev->pci->vendor_id, dev->pci->device_id, dev->name);
}

void USBSearchCB(struct pci_device* dev){
    /*
    * register device
    */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init =  &deviceInitUSB;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = USB;
    devicemgr_set_device_description(deviceinstance, "USB");
    devicemgr_register_device(deviceinstance);
}

/**
* find all USB devices and register them
*/
void usb_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_SERIAL,PCI_SERIAL_SUBCLASS_USB, &USBSearchCB);
}
