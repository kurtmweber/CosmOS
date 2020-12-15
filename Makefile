include mk/build.mk

BOOTIMAGE=hda.img

all: bootimage

bootimage: subsystems
	$(RM) $(BOOTIMAGE)
	$(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024
	$(DD) if=boot/boot.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1
	$(DD) if=boot/boot2.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2 seek=1
	$(DD) if=boot/boot3.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1 seek=3
	$(DD) if=kernel/cosmos.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2048 seek=4

subsystems: boot-subsystem kernel-subsystem
	
boot-subsystem:
	cd boot && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE)
	
clean: boot-clean kernel-clean
	$(RM) $(BOOTIMAGE)

kernel-clean:
	cd kernel && $(MAKE) clean

boot-clean:
	cd boot && $(MAKE) clean

qemu: bootimage
	$(QEMU) $(QEMUARGS)
