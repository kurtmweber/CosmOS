NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I.
MAKE=make
LD=ld
#LDFLAGS=-e CosmOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib
LDFLAGS=-T cosmos.ld -Map cosmos.map

all: kernel

kernel: cosmos

cosmos: cosmos.o asm-dir console-dir debug_error-dir interrupts-dir keyboard-dir mm-dir string-dir timing-dir video-dir
	$(LD) $(LDFLAGS) -o cosmos \
	cosmos.o \
	console/console.a \
	video/video.a \
	interrupts/interrupts.a \
	mm/mm.a \
	timing/timing.a \
	asm/asm.a \
	keyboard/keyboard.a \
	debug_error/debug_error.a \
	string/string.a

cosmos.o: cosmos.c
	$(CC) $(CFLAGS) -o cosmos.o cosmos.c

asm-dir:
	cd asm && $(MAKE)
	
console-dir:
	cd console && $(MAKE)
	
debug_error-dir:
	cd debug_error && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
keyboard-dir:
	cd keyboard && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)

string-dir:
	cd string && $(MAKE)
	
timing-dir:
	cd timing && $(MAKE)
	
video-dir:
	cd video && $(MAKE)
	
clean: kernel-clean asm-dir-clean console-dir-clean debug_error-dir-clean interrupts-dir-clean keyboard-dir-clean mm-dir-clean string-dir-clean timing-dir-clean video-dir-clean

kernel-clean: cosmos-o-clean
	$(RM) cosmos

cosmos-o-clean:
	$(RM) cosmos.o cosmos.map
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
debug_error-dir-clean:
	cd debug_error && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
keyboard-dir-clean:
	cd keyboard && $(MAKE) clean
	
mm-dir-clean:
	cd mm && $(MAKE) clean

string-dir-clean:
	cd string && $(MAKE) clean
	
timing-dir-clean:
	cd timing && $(MAKE) clean
	
video-dir-clean:
	cd video && $(MAKE) clean