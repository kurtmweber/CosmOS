include ../../mk/build.mk

EXTRA_CFLAGS= -I..

all: devs.a 

devs.a:  dev.o ata-dir keyboard-dir pci-dir serial-dir usb-dir network-dir bridge-dir display-dir rtc-dir pic-dir mouse-dir floppy-dir speaker-dir pit-dir sb16-dir ac97-dir
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
	speaker/speaker.o \
	pit/pit.o \
	sb16/sb16.o \
	ac97/ac97.o \
	dev.o
	
dev.o: dev.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

ata-dir:
	cd ata && $(MAKE)
	
keyboard-dir:
	cd keyboard && $(MAKE)

pci-dir:
	cd pci && $(MAKE)

serial-dir:
	cd serial && $(MAKE)

usb-dir:
	cd usb && $(MAKE)

network-dir:
	cd network && $(MAKE)

bridge-dir:
	cd bridge && $(MAKE)

display-dir:
	cd display && $(MAKE)

rtc-dir:
	cd rtc && $(MAKE)

pic-dir:
	cd pic && $(MAKE)

mouse-dir:
	cd mouse && $(MAKE)

floppy-dir:
	cd floppy && $(MAKE)

speaker-dir:
	cd speaker && $(MAKE)

pit-dir:
	cd pit && $(MAKE)

sb16-dir:
	cd sb16 && $(MAKE)

ac97-dir:
	cd ac97 && $(MAKE)

clean: ata-dir-clean keyboard-dir-clean pci-dir-clean serial-dir-clean usb-dir-clean network-dir-clean bridge-dir-clean display-dir-clean rtc-dir-clean pic-dir-clean mouse-dir-clean floppy-dir-clean speaker-dir-clean pit-dir-clean sb16-dir-clean ac97-dir-clean
	$(RM) devs.a
	$(RM) dev.o

ata-dir-clean:
	cd ata && $(MAKE) clean
		
keyboard-dir-clean:
	cd keyboard && $(MAKE) clean
	
pci-dir-clean:
	cd pci && $(MAKE) clean

serial-dir-clean:
	cd serial && $(MAKE) clean

usb-dir-clean:
	cd usb && $(MAKE) clean

network-dir-clean:
	cd network && $(MAKE) clean

bridge-dir-clean:
	cd bridge && $(MAKE) clean

display-dir-clean:
	cd display && $(MAKE) clean

rtc-dir-clean:
	cd rtc && $(MAKE) clean

pic-dir-clean:
	cd pic && $(MAKE) clean
 
mouse-dir-clean:
	cd mouse && $(MAKE) clean

floppy-dir-clean:
	cd floppy && $(MAKE) clean

speaker-dir-clean:
	cd speaker && $(MAKE) clean

pit-dir-clean:
	cd pit && $(MAKE) clean

sb16-dir-clean:
	cd sb16 && $(MAKE) clean

ac97-dir-clean:
	cd ac97 && $(MAKE) clean

