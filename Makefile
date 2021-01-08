include mk/build.mk

BOOTIMAGE=hda.img

all: subsystems

bootimage: subsystems
	$(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024
	$(DD) if=boot/i386/boot.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1
	$(DD) if=boot/i386/boot2.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2 seek=1
	$(DD) if=boot/i386/boot3.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1 seek=3
	$(DD) if=kernel/cosmos.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2048 seek=4

subsystems: boot-subsystem kernel-subsystem
	
boot-subsystem:
	cd boot/i386 && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE) all
	
clean: boot-clean kernel-clean
	$(RM) $(BOOTIMAGE)

kernel-clean:
	cd kernel && $(MAKE) clean

boot-clean:
	cd boot/i386 && $(MAKE) clean

qemu: bootimage
	$(QEMU) $(QEMUARGS)
