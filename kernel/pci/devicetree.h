/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DEVICETREE_H
#define _DEVICETREE_H

typedef enum pci_class_codes{
	PCI_CLASS_UNDEFINED	= 0x00,
	PCI_CLASS_MASS_STORAGE	= 0x01,
	PCI_CLASS_NETWORK	= 0x02,
	PCI_CLASS_DISPLAY	= 0x03,
	PCI_CLASS_MULTIMEDIA	= 0x04,
	PCI_CLASS_MEMORY	= 0x05,
	PCI_CLASS_BRIDGE	= 0x06,
	PCI_CLASS_SIMPLE_COMM	= 0x07,
	PCI_CLASS_BASE_PERI	= 0x08,
	PCI_CLASS_INPUT		= 0x09,
	PCI_CLASS_DOCKING	= 0x0A,
	PCI_CLASS_PROCESSOR	= 0x0B,
	PCI_CLASS_SERIAL	= 0x0C,
	PCI_CLASS_WIRELESS	= 0x0D,
	PCI_CLASS_INTELLIGENT	= 0x0E,
	PCI_CLASS_SATELLITE	= 0x0F,
	PCI_CLASS_ENCRYPTION	= 0x10,
	PCI_CLASS_SIGNAL_PROC	= 0x11,
	PCI_CLASS_ACCELERATOR	= 0x12,
	PCI_CLASS_NONESS_INST	= 0x13,
	PCI_CLASS_COPROCESSOR	= 0x40,
	PCI_CLASS_UNASSIGNED	= 0xFF
} pci_class_codes;

typedef enum pci_bridge_subclass_codes{
	PCI_BRIDGE_SUBCLASS_HOST	=	0x00,
	PCI_BRIDGE_SUBCLASS_ISA		=	0x01,
	PCI_BRIDGE_SUBCLASS_EISA	=	0x02,
	PCI_BRIDGE_SUBCLASS_MSA		=	0x03,
	PCI_BRIDGE_SUBCLASS_PCI2PCI	=	0x04,
	PCI_BRIDGE_SUBCLASS_PCMCIA	=	0x05,
	PCI_BRIDGE_SUBCLASS_NUBUS	=	0x06,
	PCI_BRIDGE_SUBCLASS_CARDBUS	=	0x07,
	PCI_BRIDGE_SUBCLASS_RACEWAY	=	0x08,
	PCI_BRIDGE_SUBCLASS_PCI2PCI_2	=	0x09,
	PCI_BRIDGE_SUBCLASS_INF2PCI	=	0x0A,
	PCI_BRIDGE_SUBCLASS_OTHER	=	0x80
} pci_bridge_subclass_codes;

typedef enum pci_display_subclass_codes{
	PCI_DISPLAY_SUBCLASS_VGA	=	0x00,
	PCI_DISPLAY_SUBCLASS_XGA	=	0x01,
	PCI_DISPLAY_SUBCLASS_3D		=	0x02,
	PCI_DISPLAY_SUBCLASS_OTHER	=	0x80
} pci_display_subclass_codes;

typedef enum pci_mass_storage_subclass_codes{
	PCI_MASS_STORAGE_SUBCLASS_SCSI		=	0x00,
	PCI_MASS_STORAGE_SUBCLASS_IDE		=	0x01,
	PCI_MASS_STORAGE_SUBCLASS_FLOPPY	=	0x02,
	PCI_MASS_STORAGE_SUBCLASS_ICI		=	0x03,
	PCI_MASS_STORAGE_SUBCLASS_RAID		=	0x04,
	PCI_MASS_STORAGE_SUBCLASS_ATA		=	0x05,
	PCI_MASS_STORAGE_SUBCLASS_SATA		=	0x06,
	PCI_MASS_STORAGE_SUBCLASS_SERIAL_SCSI	=	0x07,
	PCI_MASS_STORAGE_SUBCLASS_NVM		=	0x08,
	PCI_MASS_STORAGE_SUBCLASS_OTHER		=	0x80
} pci_mass_storage_subclass_codes;

typedef enum pci_network_subclass_codes{
	PCI_NETWORK_SUBCLASS_ETHERNET	=	0x00,
	PCI_NETWORK_SUBCLASS_TOKEN_RING	=	0x01,
	PCI_NETWORK_SUBCLASS_FDDI	=	0x02,
	PCI_NETWORK_SUBCLASS_ATM	=	0x03,
	PCI_NETWORK_SUBCLASS_ISDN	=	0x04,
	PCI_NETWORK_SUBCLASS_WORLDFIB	=	0x05,
	PCI_NETWORK_SUBCLASS_PICMG	=	0x06,
	PCI_NETWORK_SUBCLASS_INFINIBAND	=	0x07,
	PCI_NETWORK_SUBCLASS_FABRIC	=	0x08,
	PCI_NETWORK_SUBCLASS_OTHER	=	0x80
} pci_network_subclass_codes;

typedef enum pci_serial_subclass_codes{
	PCI_SERIAL_SUBCLASS_FIREWIRE	=	0x00,
	PCI_SERIAL_SUBCLASS_ACCESS	=	0x01,
	PCI_SERIAL_SUBCLASS_SSA		=	0x02,
	PCI_SERIAL_SUBCLASS_USB		=	0x03,
	PCI_SERIAL_SUBCLASS_FIBRE	=	0x04,
	PCI_SERIAL_SUBCLASS_SMBUS	=	0x05,
	PCI_SERIAL_SUBCLASS_INFINIBAND	=	0x06,
	PCI_SERIAL_SUBCLASS_IPMI	=	0x07,
	PCI_SERIAL_SUBCLASS_SERCOS	=	0x08,
	PCI_SERIAL_SUBCLASS_CANBUS	=	0x09,
	PCI_SERIAL_SUBCLASS_OTHER	=	0x80
} pci_serial_subclass_codes;

typedef struct pci_device_t pci_device_t;

typedef struct pci_device_t{
	pci_class_codes pci_class;
	uint8_t pci_subclass;
	uint8_t bus;
	uint8_t device;
	uint8_t function;
	uint8_t header_type;
	uint8_t irq;
	uint16_t vendor_id;
	uint16_t device_id;
} pci_device_t;

#ifndef _PCI_INIT_C
extern pci_device_t *pci_devices;
extern uint16_t num_pci_devices;
#else
pci_device_t *pci_devices;
uint16_t num_pci_devices;
#endif

#endif