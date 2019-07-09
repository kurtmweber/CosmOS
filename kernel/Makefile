NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0
MAKE=make
LD=ld
LDFLAGS=-e JustOS --oformat binary -Ttext 0xFFFF800000000000 -nostdlib

all: kernel

kernel: justos

justos: justos.o asm-dir
	$(LD) $(LDFLAGS) -o justos justos.o asm/asm.o

justos.o: justos.c
	$(CC) $(CFLAGS) -o justos.o justos.c

asm-dir:
	cd asm && $(MAKE)
	
clean: kernel-clean asm-dir-clean

kernel-clean: justos-o-clean
	$(RM) justos

justos-o-clean:
	$(RM) justos.o
	
asm-dir-clean:
	cd asm && $(MAKE) clean