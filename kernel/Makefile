include ../mk/build.mk

EXTRA_CFLAGS=-I.

KERNEL_ELF=cosmos.elf
KERNEL_BIN=cosmos.bin

all: kernel

kernel: cosmos

cosmos: cosmos.elf
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)

cosmos.elf: cosmos.o asm-dir ata-dir console-dir interrupts-dir keyboard-dir mm-dir panic-dir pci-dir string-dir timing-dir video-dir serial-dir device-dir usb-dir network-dir bridge-dir array-dir display-dir
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) \
	cosmos.o \
	ata/ata.a \
	pci/pci.a \
	console/console.a \
	video/video.a \
	interrupts/interrupts.a \
	mm/mm.a \
	timing/timing.a \
	asm/asm.a \
	keyboard/keyboard.a \
	panic/panic.a \
	string/string.a \
	serial/serial.a \
	device/device.a \
	usb/usb.a \
	network/network.a \
	bridge/bridge.a \
	array/array.a \
	display/display.a

cosmos.o: cosmos.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o cosmos.o cosmos.c

asm-dir:
	cd asm && $(MAKE)
	
ata-dir:
	cd ata && $(MAKE)
	
console-dir:
	cd console && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
keyboard-dir:
	cd keyboard && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)

panic-dir:
	cd panic && $(MAKE)
	
pci-dir:
	cd pci && $(MAKE)

string-dir:
	cd string && $(MAKE)
	
timing-dir:
	cd timing && $(MAKE)
	
video-dir:
	cd video && $(MAKE)

serial-dir:
	cd serial && $(MAKE)

device-dir:
	cd device && $(MAKE)

usb-dir:
	cd usb && $(MAKE)

network-dir:
	cd network && $(MAKE)

bridge-dir:
	cd bridge && $(MAKE)

array-dir:
	cd array && $(MAKE)

display-dir:
	cd display && $(MAKE)

clean: kernel-clean asm-dir-clean ata-dir-clean console-dir-clean interrupts-dir-clean keyboard-dir-clean mm-dir-clean panic-dir-clean pci-dir-clean string-dir-clean timing-dir-clean video-dir-clean serial-dir-clean device-dir-clean usb-dir-clean network-dir-clean bridge-dir-clean array-dir-clean display-dir-clean

kernel-clean: cosmos-o-clean
	$(RM) cosmos

cosmos-o-clean:
	$(RM) cosmos.o cosmos.map cosmos.elf
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
ata-dir-clean:
	cd ata && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
keyboard-dir-clean:
	cd keyboard && $(MAKE) clean
	
mm-dir-clean:
	cd mm && $(MAKE) clean
	
panic-dir-clean:
	cd panic && $(MAKE) clean

pci-dir-clean:
	cd pci && $(MAKE) clean

string-dir-clean:
	cd string && $(MAKE) clean

serial-dir-clean:
	cd serial && $(MAKE) clean

timing-dir-clean:
	cd timing && $(MAKE) clean
	
video-dir-clean:
	cd video && $(MAKE) clean

device-dir-clean:
	cd device && $(MAKE) clean

usb-dir-clean:
	cd usb && $(MAKE) clean

network-dir-clean:
	cd network && $(MAKE) clean

bridge-dir-clean:
	cd bridge && $(MAKE) clean

array-dir-clean:
	cd array && $(MAKE) clean

display-dir-clean:
	cd display && $(MAKE) clean