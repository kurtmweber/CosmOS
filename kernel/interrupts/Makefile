include ../../mk/build.mk

CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ISRCFLAGS=-mgeneral-regs-only
AR=ar
ARFLAGS=-crs
MAKE=make

all: interrupts.a

interrupts.a: de.o idt.o irq.o pic.o
	$(AR) $(ARFLAGS) interrupts.a de.o idt.o irq.o pic.o
	
de.o: de.c
	$(CC) $(CFLAGS) $(ISRCFLAGS) -o $@ $<

idt.o: idt.c
	$(CC) $(CFLAGS) -o $@ $<
	
irq.o: irq.c
	$(CC) $(CFLAGS) $(ISRCFLAGS) -o $@ $<
	
pic.o: pic.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean: de-o-clean interrupts-a-clean idt-o-clean irq-o-clean pic-o-clean

de-o-clean:
	$(RM) de.o

interrupts-a-clean:
	$(RM) interrupts.a

idt-o-clean:
	$(RM) idt.o

irq-o-clean:
	$(RM) irq.o
	
pic-o-clean:
	$(RM) pic.o