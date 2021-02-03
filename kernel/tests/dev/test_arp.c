//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/tcpip/arp/arpdev.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_arp.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_arp.h>

void test_arp() {
    uint8_t devicename[] = {"arp0"};

    struct device* arp = devicemgr_find_device(devicename);
    if (0 != arp) {
        struct deviceapi_arp* arp_api = (struct deviceapi_arp*)arp->api;

        struct arp arp_request;
        arp_packet_init(&arp_request, ARP_REQUEST);

        struct arp arp_response;
        memzero((uint8_t*)&arp_response, sizeof(struct arp));

        (*arp_api->request)(arp, &arp_request, &arp_response);

        debug_show_memblock((uint8_t*)&arp_response, sizeof(struct arp));

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
