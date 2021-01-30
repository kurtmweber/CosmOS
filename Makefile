include mk/build.mk

BOOTIMAGE=img/hda.img
BLANK_DISK=img/blank.img
SRC_FILES=$(shell find . -type f \( -name "*.c" -o -name "*.h" \))

CRUFT_FILES=$(shell find . -type f \( -name "dump.dat" -o \
									  -name "qemu.log" \))

all: subsystems
.PHONY: clean

bootimage: subsystems
	$(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024
	$(DD) if=boot/x86-64/boot.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1
	$(DD) if=boot/x86-64/boot2.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2 seek=1
	$(DD) if=boot/x86-64/boot3.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=1 seek=3
	$(DD) if=kernel/cosmos.bin of=$(BOOTIMAGE) conv=notrunc bs=512 count=2048 seek=4

subsystems: boot-subsystem kernel-subsystem blank-disk
	
blank-disk:
	$(DD) if=/dev/zero of=$(BLANK_DISK) bs=1024 count=10240

boot-subsystem:
	cd boot/x86-64 && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE) all
	
clean: boot-clean kernel-clean
	$(RM) $(BOOTIMAGE)
	$(RM) $(CRUFT_FILES)

kernel-clean:
	cd kernel && $(MAKE) clean

boot-clean:
	cd boot/x86-64 && $(MAKE) clean

qemu: bootimage
	$(QEMU) $(QEMUARGS)

qemu-debug: bootimage
	$(QEMU) $(QEMUARGS) $(QEMUDEBUGARGS)

lint:
	clang-format -n -style=file $(SRC_FILES)