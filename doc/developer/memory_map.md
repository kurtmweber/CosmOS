
# CosmOS Memory Map

Cosmos boots from boot.asm, at location 0x7C00.  The second stage boot loader, boot2.asm is at 0x7E00, and the third stage boot loader, boot loader, is at 0x8200.  

The kernel page table starts at 0x100000 and the kernel starts at 0xFFFF800000000000.
