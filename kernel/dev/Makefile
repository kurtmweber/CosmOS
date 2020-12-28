include ../../mk/build.mk

EXTRA_CFLAGS= -I..

all: devs.a 

# this finds the names of all subdirectories under this one, that contain a Makefile
SUBDIRS=$(shell find . -mindepth 2 -type f -name 'Makefile' | awk -F '/' '{print $$2}')
$(info $$SUBDIRS is [${SUBDIRS}])

devs.a: dev.o build-subprojects
	$(AR) $(ARFLAGS)T $@ \
	ata/ata.a \
	pci/pci.a \
	keyboard/keyboard.a \
	serial/serial.a \
	usb/usb.a \
	network/network.a \
	bridge/bridge.a \
	display/display.a \
	rtc/rtc.a \
	pic/pic.a \
	mouse/mouse.a \
	floppy/floppy.a \
	speaker/speaker.a \
	pit/pit.a \
	sb16/sb16.a \
	ac97/ac97.a \
	adlib/adlib.a \
	cmos/cmos.a \
	isadma/isadma.a \
	cpu/cpu.a \
	virtio/virtio.a \
	dev.o
	
dev.o: dev.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

build-subprojects:
	$(foreach file, $(SUBDIRS), cd $(file) && make && cd ..;)


clean: clean-subprojects
	$(RM) devs.a
	$(RM) dev.o

clean-subprojects:
	$(foreach file, $(SUBDIRS), cd $(file) && make clean && cd ..;)
