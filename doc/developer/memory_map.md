
# CosmOS Memory Map

Cosmos boots from boot.asm, at location 0x7C00.  The second stage boot loader, boot2.asm is at 0x7E00, and the third stage boot loader, boot loader, is at 0x8200.

At kernel load, physical memory is mapped as follows:

0x00000000 - 0x000FFFFF (1 megabyte)        Identity mapped.
                                            This includes many page tables and system data, and should not be used for any other purposes without serious discussion and planning
        0x500 - as needed                   INT 15h memory map
        0x10000 - 0x7FFFF (448 kilobytes)   Early kernel page tables.  Must not be overwritten.
0x00000000 - 0x00FFFFFF (15 megabytes)      Unused, accessible via direct map.  Available for IO buffers and other uses.
0x01000000 - 0x017FFFFF (8 megabytes)       Kernel stack, mapped at 0xFFFFFFFFFF800000
0x01800000 - 0x01FFFFFF (8 megabytes)       Kernel heap + text, mapped at 0xFFFF800000000000