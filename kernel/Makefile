include ../mk/build.mk

EXTRA_CFLAGS=-I.

KERNEL_ELF=cosmos.elf
KERNEL_BIN=cosmos.bin

all: kernel

kernel: cosmos

cosmos: $(KERNEL_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)

$(KERNEL_ELF): cosmos.o asm-dir ata-dir console-dir interrupts-dir keyboard-dir mm-dir panic-dir pci-dir string-dir sleep-dir video-dir serial-dir devicemgr-dir usb-dir network-dir bridge-dir collection-dir display-dir rtc-dir pic-dir ethernet-dir
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) \
	cosmos.o \
	dev/ata/ata.a \
	dev/pci/pci.a \
	console/console.a \
	video/video.a \
	interrupts/interrupts.a \
	mm/mm.a \
	sleep/sleep.a \
	asm/asm.a \
	dev/keyboard/keyboard.a \
	panic/panic.a \
	string/string.a \
	dev/serial/serial.a \
	devicemgr/devicemgr.a \
	dev/usb/usb.a \
	dev/network/network.a \
	dev/bridge/bridge.a \
	collection/collection.a \
	dev/display/display.a \
	dev/rtc/rtc.o \
	dev/pic/pic.o \
	ethernet/ethernet.o

cosmos.o: cosmos.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o cosmos.o cosmos.c

asm-dir:
	cd asm && $(MAKE)
	
ata-dir:
	cd dev/ata && $(MAKE)
	
console-dir:
	cd console && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
keyboard-dir:
	cd dev/keyboard && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)

panic-dir:
	cd panic && $(MAKE)
	
pci-dir:
	cd dev/pci && $(MAKE)

string-dir:
	cd string && $(MAKE)
	
sleep-dir:
	cd sleep && $(MAKE)
	
video-dir:
	cd video && $(MAKE)

serial-dir:
	cd dev/serial && $(MAKE)

devicemgr-dir:
	cd devicemgr && $(MAKE)

usb-dir:
	cd dev/usb && $(MAKE)

network-dir:
	cd dev/network && $(MAKE)

bridge-dir:
	cd dev/bridge && $(MAKE)

collection-dir:
	cd collection && $(MAKE)

display-dir:
	cd dev/display && $(MAKE)

rtc-dir:
	cd dev/rtc && $(MAKE)

pic-dir:
	cd dev/pic && $(MAKE)

ethernet-dir:
	cd ethernet && $(MAKE)

clean: kernel-clean asm-dir-clean ata-dir-clean console-dir-clean interrupts-dir-clean keyboard-dir-clean mm-dir-clean panic-dir-clean pci-dir-clean string-dir-clean sleep-dir-clean video-dir-clean serial-dir-clean devicemgr-dir-clean usb-dir-clean network-dir-clean bridge-dir-clean collection-dir-clean display-dir-clean rtc-dir-clean pic-dir-clean ethernet-dir-clean

kernel-clean: cosmos-o-clean
	$(RM) cosmos

cosmos-o-clean:
	$(RM) cosmos.o cosmos.map cosmos.elf
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
ata-dir-clean:
	cd dev/ata && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
keyboard-dir-clean:
	cd dev/keyboard && $(MAKE) clean
	
mm-dir-clean:
	cd mm && $(MAKE) clean
	
panic-dir-clean:
	cd panic && $(MAKE) clean

pci-dir-clean:
	cd dev/pci && $(MAKE) clean

string-dir-clean:
	cd string && $(MAKE) clean

serial-dir-clean:
	cd dev/serial && $(MAKE) clean

sleep-dir-clean:
	cd sleep && $(MAKE) clean
	
video-dir-clean:
	cd video && $(MAKE) clean

devicemgr-dir-clean:
	cd devicemgr && $(MAKE) clean

usb-dir-clean:
	cd dev/usb && $(MAKE) clean

network-dir-clean:
	cd dev/network && $(MAKE) clean

bridge-dir-clean:
	cd dev/bridge && $(MAKE) clean

collection-dir-clean:
	cd collection && $(MAKE) clean

display-dir-clean:
	cd dev/display && $(MAKE) clean

rtc-dir-clean:
	cd dev/rtc && $(MAKE) clean

pic-dir-clean:
	cd dev/pic && $(MAKE) clean
 
ethernet-dir-clean:
	cd ethernet && $(MAKE) clean