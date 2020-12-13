include ../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I.

all: kernel

kernel: cosmos

cosmos: cosmos.o asm-dir ata-dir console-dir interrupts-dir keyboard-dir mm-dir panic-dir pci-dir string-dir timing-dir video-dir
	$(LD) $(LDFLAGS) -o cosmos \
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
	string/string.a

cosmos.o: cosmos.c
	$(CC) $(CFLAGS) -o cosmos.o cosmos.c

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
	
clean: kernel-clean asm-dir-clean ata-dir-clean console-dir-clean interrupts-dir-clean keyboard-dir-clean mm-dir-clean panic-dir-clean pci-dir-clean string-dir-clean timing-dir-clean video-dir-clean

kernel-clean: cosmos-o-clean
	$(RM) cosmos

cosmos-o-clean:
	$(RM) cosmos.o cosmos.map
	
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
	
timing-dir-clean:
	cd timing && $(MAKE) clean
	
video-dir-clean:
	cd video && $(MAKE) clean