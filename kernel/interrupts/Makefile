NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: interrupts.a

interrupts.a: idt.o
	$(AR) $(AFLAGS) interrupts.a idt.o

idt.o: idt.c
	$(CC) $(CFLAGS) -o idt.o idt.c
	
clean: interrupts-a-clean idt-o-clean

interrupts-a-clean:
	$(RM) interrupts.a

idt-o-clean:
	$(RM) idt.o