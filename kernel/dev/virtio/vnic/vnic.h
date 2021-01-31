//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VNIC_H
#define _VNIC_H

#include <types.h>

// Network-device-specific registers:
#define REG_MAC_1 0x14
#define REG_MAC_2 0x15
#define REG_MAC_3 0x16
#define REG_MAC_4 0x17
#define REG_MAC_5 0x18
#define REG_MAC_6 0x19
#define REG_NIC_STATUS 0x1A

// Feature bits (See 5.1.3 of virtio-v1.0-cs04.pdf)
#define VIRTIO_NET_F_CSUM 0
#define VIRTIO_NET_F_GUEST_CSUM 1
#define VIRTIO_NET_F_CTRL_GUEST_OFFLOADS 2
#define VIRTIO_NET_F_MAC 0x20
#define VIRTIO_NET_F_GUEST_TSO4 0x80
#define VIRTIO_NET_F_GUEST_TSO6 0x100
#define VIRTIO_NET_F_GUEST_ECN 0x200
#define VIRTIO_NET_F_GUEST_UFO 0x400
#define VIRTIO_NET_F_HOST_TSO4 0x800
#define VIRTIO_NET_F_HOST_TSO6 0x1000
#define VIRTIO_NET_F_HOST_ECN 0x2000
#define VIRTIO_NET_F_HOST_UFO 0x4000
#define VIRTIO_NET_F_MRG_RXBUF 0x8000
#define VIRTIO_NET_F_STATUS 0x10000
#define VIRTIO_NET_F_CTRL_VQ 0x20000
#define VIRTIO_NET_F_CTRL_RX 0x40000
#define VIRTIO_NET_F_CTRL_VLAN 0x80000
#define VIRTIO_NET_F_GUEST_ANNOUNCE 0x200000
#define VIRTIO_NET_F_MQ 0x400000
#define VIRTIO_NET_F_CTRL_MAC_ADDR 0x800000
#define VIRTIO_F_ANY_LAYOUT (1 << 27)

// These are the features required by this driver
#undef VIRTIO_NET_REQUIRED_FEATURES
#define VIRTIO_NET_REQUIRED_FEATURES (/*VIRTIO_NET_F_CSUM |*/ VIRTIO_NET_F_MAC | VIRTIO_NET_F_MRG_RXBUF)

// Flags for virtio_net_hdr
#define VIRTIO_NET_HDR_F_NEEDS_CSUM 1
#define VIRTIO_NET_HDR_F_DATA_VALID 2
#define VIRTIO_NET_HDR_F_RSC_INFO 4

// Values for virtio_net_hdr gso_type
#define VIRTIO_NET_HDR_GSO_NONE 0
#define VIRTIO_NET_HDR_GSO_TCPV4 1
#define VIRTIO_NET_HDR_GSO_UDP 3
#define VIRTIO_NET_HDR_GSO_TCPV6 4
#define VIRTIO_NET_HDR_GSO_ECN 0x80

// Indexes of virtqs on device. See // see 4.1.5.1.3 of virtio-v1.0-cs04.pdf
#define VIRTQ_NET_RECEIVE_INDEX 0
#define VIRTQ_NET_TRANSMIT_INDEX 1

// Every packet sent or received must be preceded by a virtio_net_hdr
typedef struct virtio_net_hdr {
    uint8_t flags;
    uint8_t gso_type;
    uint16_t hdr_len;
    uint16_t gso_size;
    uint16_t csum_start;
    uint16_t csum_offset;
    uint16_t num_buffers;  // num_buffer is not part of struct if VIRTIO_NET_F_MRG_RXBUF isn't negotiated
} virtio_net_hdr;

struct vnic_devicedata {
    uint64_t base;
    struct virtq* send_queue;
    struct virtq* recieve_queue;
} __attribute__((packed));

void devicemgr_register_vnic_devices();
uint32_t vnic_read_register(uint16_t reg);
void vnic_write_register(uint16_t reg, uint32_t data);

#endif