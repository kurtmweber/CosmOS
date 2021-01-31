//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <dev/virtio/virtio.h>
#include <dev/virtio/virtqueue.h>
#include <dev/virtio/vnic/vnic.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <types.h>

uint16_t vnet_base_port;
uint8_t mac_addr[6];

void vnic_irq_handler(stackFrame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("#");
}

inline uint32_t vnic_read_register(uint16_t reg) {
    // if 4-byte register
    if (reg < VIRTIO_QUEUE_SIZE) {
        return asm_in_d(vnet_base_port + reg);
    } else {
        // if 2-byte register
        if (reg <= VIRTIO_QUEUE_NOTIFY)
            return asm_in_w(vnet_base_port + reg);
        else  // 1-byte register
            return asm_in_b(vnet_base_port + reg);
    }
}

inline uint32_t vnic_write_register(uint16_t reg, uint32_t data) {
    // if 4-byte register
    if (reg < VIRTIO_QUEUE_SIZE) {
        asm_out_d(vnet_base_port + reg, data);
    } else {
        // if 2-byte register
        if (reg <= VIRTIO_QUEUE_NOTIFY)
            asm_out_w(vnet_base_port + reg, (uint16_t)data);
        else  // 1-byte register
            asm_out_b(vnet_base_port + reg, (uint8_t)data);
    }
}

void vnic_init_virtqueue(struct virtq* virtqueue, uint16_t queueIndex) {
    int16_t queue_size = -1;

    // get queue size from the device register
    vnic_write_register(VIRTIO_QUEUE_SELECT, queueIndex);
    queue_size = (uint16_t)vnic_read_register(VIRTIO_QUEUE_SIZE);

    if (!queue_size)
        panic("Can't get Virtio network queue size");

    kprintf("\n      queue %d has %d elements", queueIndex, queue_size);

    // make the queue
    struct virtq* q = virtq_new(queue_size);
    bool q_aligned = virtio_isAligned(((uint64_t)q), 4096);
    ASSERT(q_aligned);
    virtqueue = q;

    // divide by 4096
    // The API takes a 32 bit pointer, but we have a 64 bit pointer, so ... some conversions
    uint32_t q_shifted = (uint64_t)q >> 12;

    kprintf("       Queue Address(%d): %#hX %#hX\n", queueIndex, q, q_shifted);

    // Write addresses (divided by 4096) to address registers
    vnic_write_register(VIRTIO_QUEUE_ADDRESS, q_shifted);
}

uint8_t vnic_initialize_device(struct device* dev) {
    struct vnic_devicedata* deviceData = (struct vnic_devicedata*)dev->deviceData;

    // get the I/O port
    deviceData->base = pci_calcbar(dev->pci);
    vnet_base_port = deviceData->base;

    kprintf("    Initializing virtio-net driver... Base address: %#hX", vnet_base_port);

    // Reset the virtio-network device
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_RESET_DEVICE);

    // Set the acknowledge status bit
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // Set the driver status bit
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED | VIRTIO_STATUS_DRIVER_LOADED);

    // Read the feature bits
    uint32_t features = vnic_read_register(VIRTIO_DEVICE_FEATURES);

    // Make sure the features we need are supported
    if ((features & VIRTIO_NET_REQUIRED_FEATURES) != VIRTIO_NET_REQUIRED_FEATURES) {
        // uh-oh
        kprintf("Required features are not supported by virtio network device. Aborting.\n");
        vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ERROR);
        return;
    }

    // Tell the device what features we'll be using
    vnic_write_register(VIRTIO_GUEST_FEATURES, VIRTIO_NET_REQUIRED_FEATURES);

    // Tell the device the features have been negotiated
    vnic_write_register(VIRTIO_DEVICE_STATUS,
                        VIRTIO_STATUS_DEVICE_ACKNOWLEGED | VIRTIO_STATUS_DRIVER_LOADED | VIRTIO_STATUS_FEATURES_OK);

    // Make sure the device is ok with those features
    if ((vnic_read_register(VIRTIO_DEVICE_STATUS) & VIRTIO_STATUS_FEATURES_OK) != VIRTIO_STATUS_FEATURES_OK) {
        // uh-oh
        kprinf("Failed to negotiate features with virtio net device. Aborting.");
        vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ERROR);
        return;
    }

    // store the MAC address
    // TODO: store mac in the device struct
    uint16_t macReg = REG_MAC_1;
    for (int i = 0; i < 6; ++i, ++macReg)
        mac_addr[i] = (uint8_t)vnic_read_register(macReg);

    kprintf("   MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
            mac_addr[5]);

    // Init virtqueues (see 4.1.5.1.3 of virtio-v1.0-cs04.pdf)
    vnic_init_virtqueue(deviceData->recieve_queue, VIRTQ_NET_RECEIVE_INDEX);
    vnic_init_virtqueue(deviceData->send_queue, VIRTQ_NET_TRANSMIT_INDEX);

    // Setup the receive queue
    // VirtIO_Net_SetupReceiveBuffers();

    // Setup an interrupt handler for this device
    interrupt_router_register_interrupt_handler(dev->pci->irq, &vnic_irq_handler);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, deviceData->base, dev->name);

    // Tell the device it's initialized
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DRIVER_READY);

    // Remind the device that it has receive buffers.
    vnic_write_register(VIRTIO_QUEUE_NOTIFY, VIRTQ_NET_RECEIVE_INDEX);

    // kprintf("\n     Requesting IP address via DHCP...");
    //ARP_SendRequest(IPv4_PackIP(10, 0, 2, 4), mac_addr);
    // DHCP_Send_Discovery(mac_addr);

    kprintf("virtio-net driver initialized.");
}

void vnic_ethernet_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    panic("vnic read not implemented yet");
}

void vnic_ethernet_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    panic("vnic write not implemented yet");
}

// As part of PCI discovery, devicemgr calls this to register us as an instance of type VNIC.
// We create a device instance, attach an API and data storage, and register it.
void devicemgr_register_pci_vnic(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);

    // create a new device
    struct device* deviceinstance = devicemgr_new_device();

    // bind an initialization function to the device (called by devicemgr during startup)
    deviceinstance->init = &vnic_initialize_device;

    // pci device info (including IRQ)
    deviceinstance->pci = dev;

    // set properties
    deviceinstance->devicetype = VNIC;
    devicemgr_set_device_description(deviceinstance, "Virtio NIC");

    // define an api
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*)kmalloc(sizeof(struct deviceapi_ethernet));
    api->write = &vnic_ethernet_write;
    api->read = &vnic_ethernet_read;
    deviceinstance->api = api;

    // reserve for device-specific data
    struct vnic_devicedata* deviceData = (struct vnic_devicedata*)kmalloc(sizeof(struct vnic_devicedata));
    deviceinstance->deviceData = deviceData;

    // register
    devicemgr_register_device(deviceinstance);
}

// find all virtio ethernet devices and register them
void devicemgr_register_vnic_devices() {
    pci_devicemgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, VIRTIO_PCI_MANUFACTURER,
                                VIRTIO_PCI_DEVICED_NETWORK, &devicemgr_register_pci_vnic);
}
