include ../mk/build.mk

EXTRA_CFLAGS=-I.

KERNEL_ELF=cosmos.elf
KERNEL_BIN=cosmos.bin

all: kernel

kernel: cosmos

cosmos: $(KERNEL_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(KERNEL_ELF) $(KERNEL_BIN)

$(KERNEL_ELF): dev-subsystem cosmos.o asm-dir console-dir interrupts-dir mm-dir panic-dir string-dir sleep-dir video-dir devicemgr-dir collection-dir ethernet-dir
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) \
	cosmos.o \
	console/console.a \
	video/video.a \
	interrupts/interrupts.a \
	mm/mm.a \
	sleep/sleep.a \
	asm/asm.a \
	panic/panic.a \
	string/string.a \
	devicemgr/devicemgr.a \
	ethernet/ethernet.a \
	dev/devs.a \
	collection/collection.a 

dev-subsystem:
	cd dev && $(MAKE)

cosmos.o: cosmos.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o cosmos.o cosmos.c

asm-dir:
	cd asm && $(MAKE)
		
console-dir:
	cd console && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)

panic-dir:
	cd panic && $(MAKE)
	
string-dir:
	cd string && $(MAKE)
	
sleep-dir:
	cd sleep && $(MAKE)

video-dir:
	cd video && $(MAKE)

devicemgr-dir:
	cd devicemgr && $(MAKE)

collection-dir:
	cd collection && $(MAKE)

ethernet-dir:
	cd ethernet && $(MAKE)

clean: dev-clean kernel-clean asm-dir-clean console-dir-clean interrupts-dir-clean mm-dir-clean panic-dir-clean string-dir-clean sleep-dir-clean video-dir-clean devicemgr-dir-clean collection-dir-clean ethernet-dir-clean

dev-clean:
	cd dev && $(MAKE) clean

kernel-clean: cosmos-o-clean
	$(RM) cosmos

cosmos-o-clean:
	$(RM) cosmos.o cosmos.map cosmos.elf
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
mm-dir-clean:
	cd mm && $(MAKE) clean
	
panic-dir-clean:
	cd panic && $(MAKE) clean

string-dir-clean:
	cd string && $(MAKE) clean

sleep-dir-clean:
	cd sleep && $(MAKE) clean
	
video-dir-clean:
	cd video && $(MAKE) clean

devicemgr-dir-clean:
	cd devicemgr && $(MAKE) clean

collection-dir-clean:
	cd collection && $(MAKE) clean

ethernet-dir-clean:
	cd ethernet && $(MAKE) clean


