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

justos: justos.o asm-dir console-dir interrupts-dir mm-dir
	$(LD) $(LDFLAGS) -o justos justos.o asm/asm.o console/console.a interrupts/interrupts.a mm/mm.a

justos.o: justos.c
	$(CC) $(CFLAGS) -o justos.o justos.c

asm-dir:
	cd asm && $(MAKE)
	
console-dir:
	cd console && $(MAKE)

interrupts-dir:
	cd interrupts && $(MAKE)
	
mm-dir:
	cd mm && $(MAKE)
	
clean: kernel-clean asm-dir-clean console-dir-clean interrupts-dir-clean

kernel-clean: justos-o-clean
	$(RM) justos

justos-o-clean:
	$(RM) justos.o
	
asm-dir-clean:
	cd asm && $(MAKE) clean
	
console-dir-clean:
	cd console && $(MAKE) clean
	
interrupts-dir-clean:
	cd interrupts && $(MAKE) clean
	
memory-dir-clean:
	cd memory && $(MAKE) clean