//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OHCI_H
#define _OHCI_H

#define OHCI_HC_REVISION 0x00
#define OHCI_HC_CONTROL 0x04
#define OHCI_HC_COMMANDSTATUS 0x08
#define OHCI_HC_INTERRUPTSTATUS 0x0C
#define OHCI_HC_INTERRUPTENABLE 0x10
#define OHCI_HC_INTERRUPTDISABLE 0x14
#define OHCI_HC_HCCA 0x18
#define OHCI_HC_PERIODCURRENTED 0x1C
#define OHCI_HC_CONTROLHEADED 0x20
#define OHCI_HC_CONTROLCURRENTED 0x24
#define OHCI_HC_BULKHEADED 0x28
#define OHCI_HC_BULKCURRENTED 0x2C
#define OHCI_HC_DONEHEAD 0x30
#define OHCI_HC_FMINTERVAL 0x34
#define OHCI_HC_FMREMAINING 0x38
#define OHCI_HC_FMNUMBER 0x3C
#define OHCI_HC_PERIODICSTART 0x40
#define OHCI_HC_LSTHRESHOLD 0x44
#define OHCI_HC_RHDESCRIPTORA 0x48
#define OHCI_HC_RHDESCRIPTORB 0x4C
#define OHCI_HC_RHSTATUS 0x50

#endif