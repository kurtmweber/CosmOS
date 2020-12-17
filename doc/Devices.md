
The CosmOS strategy for detecting an initializing devices is below

# DeviceManager

the CosmOS device manager at `/devicemgr/devicemgr.h` contains a database of phsyical devices (PCI, non-PCI, etc).

# Registration

* Call the "register" function for each device type. For example `network_register_devices()` registers the network devices.

* The device type register function for the type calls register functions for each driver. For example

```
void network_register_devices() {
    rtl8139_register_devices();
    ne2000_register_devices();
}
```

Each device driver's register function finds all instance of the specific device type, and calls `void registerDevice(struct device* dev);` to register each instance.

Device's are on the PCI bus can use the PCI search functions to find device instances

```
void pci_search_device(pci_class,pci_subclass, vendor_id, device_id, deviceSearchCallback cb);
void pci_search_devicetype(pci_class,pci_subclass, deviceSearchCallback cb);
```

The callback function `typedef void (*deviceSearchCallback)(struct pci_device* dev);` is called once for each device instance found in the PCI database.

Non-PCI bus devices such as Keyboard, RTC, and RS-232 will need to use hard-coded configuration.

PCI devices generally use the `deviceData` field of the `device` struct to contain their appropriate `pci_device` struct.  Non-PCI devices can use this field to hold a pointer to their own custom configuration.


# Initialization

The function `void initDevices();` is used by the device manager to initialize all devices. The devices will have passed a pointer to a init function with this prototype:

`typedef void (*deviceInit)(struct device* dev);`
