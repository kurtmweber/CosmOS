NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
MAKE=make

all: asm.o

asm.o: asm.c
	$(CC) $(CFLAGS) -o asm.o asm.c
	
clean: asm-o-clean

asm-o-clean:
	$(RM) asm.o