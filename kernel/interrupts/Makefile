NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ISRCFLAGS=-mgeneral-regs-only
AR=ar
AFLAGS=-crs
MAKE=make

all: interrupts.a

interrupts.a: de.o idt.o
	$(AR) $(AFLAGS) interrupts.a de.o idt.o
	
de.o: de.c
	$(CC) $(CFLAGS) $(ISRCFLAGS) -o $@ $<

idt.o: idt.c
	$(CC) $(CFLAGS) -o idt.o idt.c
	
clean: de-o-clean interrupts-a-clean idt-o-clean

de-o-clean:
	$(RM) de.o

interrupts-a-clean:
	$(RM) interrupts.a

idt-o-clean:
	$(RM) idt.o