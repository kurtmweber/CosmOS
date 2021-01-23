
# CosmOS device status

## Working

* PS/2 keyboard
* PS/2 mouse
* ATA (PIO only)
* CMOS
* CPU
* PCI bus
* PIC
* PIT
* RTC
* RS-232 Serial
* PC Speaker
* VGA
* Serial Console
* VGA Console
* RAM Disk
* BDA (Bios Data Area)
* Parallel port
* Swap device
* MBR partition table and paritions

## In Development

* AC97 sound.  Device is stubbed in and dectected on the PCI bus.
* Adlib sound.  Device is stubbed in.  ISA driver not written.
* SB16 sound.  Device is ISA. Driver is stubbed in, waiting on working ISA DMA.
* Bridges. Three PCI bridges are stubbed in.
* Floppy.  ISA floppy support is stubbed in, waiting on working ISA DMA
* E1000 network.  Device is PCI and is detected.
* NE2000 ISA network.  Device is stubbed in.
* NE2000 PCI network.  Device is stubbed in.
* RTL8139 network. Device is detected on PCI bus.
* USB. Device is detected on PCI network.
* Virtio Network. Device is detected on PCI network. Support is waiting on working virtq.
* Virtio Disk. Support is waiting on working virtq.
* TFS filesystem
* FAT filesystem
* CFS filesystem
* SFS filesystem 
* GUID partition table
* ACPI.  We can get the physical address of the ACPI table but cant translate it to virtual address yet
