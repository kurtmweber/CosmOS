//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SMBIOS_H
#define _SMBIOS_H

#include <types.h>

struct device;

struct sm_bios_entry_point {
    uint8_t entry_point_string[4];  //This is _SM_
    uint8_t checksum;               //This value summed with all the values of the table, should be 0 (overflow)
    uint8_t length;                 //Length of the Entry Point Table. Since version 2.1 of SMBIOS, this is 0x1F
    uint8_t major_version;          //Major Version of SMBIOS
    uint8_t minor_version;          //Minor Version of SMBIOS
    uint16_t max_structure_size;    //Maximum size of a SMBIOS Structure (we will se later)
    uint8_t entry_point_revision;   //...
    char formatted_area[5];         //...
    char entry_point_string2[5];    //This is _DMI_
    uint8_t checksum2;              //Checksum for values from EntryPointString2 to the end of table
    uint16_t table_length;          //Length of the Table containing all the structures
    uint32_t table_address;         //Address of the Table
    uint16_t number_of_structures;  //Number of structures in the table
    uint8_t bcd_revision;           //Unused
} __attribute__((packed));

void smbios_devicemgr_register_devices();

struct sm_bios_entry_point* smbios_get_sm_bios_entry_point(struct device* dev);

#endif