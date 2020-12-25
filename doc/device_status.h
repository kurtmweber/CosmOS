
# CosmOS device status

* PS/2 keyboard. Works.
* PS/2 mouse. Works.
* ATA.  Device is detected on PCI network. Some support is written.
* AC97 sound.  Device is stubbed in and dectected on the PCI bus.
* Adlib sound.  Device is stubbed in.  ISA driver not written.
* SB16 sound.  Device is ISA. Driver is stubbed in, waiting on working ISA DMA.
* Bridges. Three PCI bridges are stubbed in.
* CMOS.  Device works.
* CPU. Device works.
* Display.  PCI display is detected
* Floppy.  ISA floppy support is stubbed in, waiting on working ISA DMA
* E1000 network.  Device is PCI and is detected.
* NE2000 ISA network.  Device is stubbed in.
* NE2000 PCI network.  Device is stubbed in.
* RTL8139 network. Device is detected on PCI bus.
* PIC. Device works.
* PIT. Device works.
* RTC. Device works.
* RS-232 Serial.  Device works.
* PC Speaker.  Device works.
* USB. Device is detected on PCI network.
* Virtio Network. Device is detected on PCI network. Support is waiting on working virtq.
* Virtio Disk. Support is waiting on working virtq.