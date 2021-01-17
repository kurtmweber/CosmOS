//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Enhanced_Host_Controller_Interface

#ifndef _EHCI_H
#define _EHCI_H

#define USB_EHCI_CAPABILITY_REGISTER 0x00
#define USB_EHCI_HCIVERSION_REGISTER 0x02
#define USB_EHCI_HCSPARAMS_REGISTER 0x04
#define USB_EHCI_HCCPARAMS_REGISTER 0x08
#define USB_EHCI_HCSP_PORTROUTE_REGISTER 0x0C

#define USB_EHCI_USBCMD_OPERATIONAL_REGISTER 0x00
#define USB_EHCI_USBSTS_OPERATIONAL_REGISTER 0x04
#define USB_EHCI_USBINTR_OPERATIONAL_REGISTER 0x08
#define USB_EHCI_FRINDEX_OPERATIONAL_REGISTER 0x0C

#define USB_EHCI_CTRLDSSEGMENT_OPERATIONAL_REGISTER 0x10
#define USB_EHCI_PERIODICLISTBASE_OPERATIONAL_REGISTER 0x14
#define USB_EHCI_ASYNCLISTADDR_OPERATIONAL_REGISTER 0x18

#define USB_EHCI_CONFIGFLAG_OPERATIONAL_REGISTER 0x40
#define USB_EHCI_PORTSC_OPERATIONAL_REGISTER 0x44

#endif