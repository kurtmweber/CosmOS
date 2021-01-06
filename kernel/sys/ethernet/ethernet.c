//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/ethernet/ethernet.h>

struct ethernet_header {
    uint8_t destinationMAC[6];
    uint8_t sourceMAC[6];
    uint16_t ethertype;
    uint8_t* payload;
} __attribute__((packed));


void send(uint8_t* destinationMAC, uint8_t* packet) {

}

uint8_t* receive() {


}