NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I.
MAKE=make
LD=ld
#LDFLAGS=-e JustOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib
LDFLAGS=-T justos.ld -Map justos.map

all: kernel

kernel: justos

justos: justos.o asm-dir console-dir debug_error-dir interrupts-dir mm-dir string-dir
	$(LD) $(LDFLAGS) -o justos justos.o asm/asm.o console/console.a interrupts/interrupts.a mm/mm.a debug_error/debug_error.a string/string.a

justos.o: justos.c
	$(CC) $(CFLAGS) -o justos.o justos.c

asm-dir:
	cd asm && $(MAKE)
	
console-dir:
	cd console && $(MAKE)
	
debug_error-dir:
	cd debug_error && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)

string-dir:
	cd string && $(MAKE)
	
clean: kernel-clean asm-dir-clean console-dir-clean debug_error-dir-clean interrupts-dir-clean mm-dir-clean string-dir-clean

kernel-clean: justos-o-clean
	$(RM) justos

justos-o-clean:
	$(RM) justos.o
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
debug_error-dir-clean:
	cd debug_error && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
mm-dir-clean:
	cd mm && $(MAKE) clean

string-dir-clean:
	cd string && $(MAKE) clean