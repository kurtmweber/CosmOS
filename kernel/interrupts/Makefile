include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ISRCFLAGS=-mgeneral-regs-only
ARFLAGS=-crs

all: interrupts.a

interrupts.a: de.o idt.o irq.o pic.o
	$(AR) $(ARFLAGS) interrupts.a de.o idt.o irq.o pic.o
	
de.o: de.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $<

idt.o: idt.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
irq.o: irq.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $<
	
pic.o: pic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean:
	$(RM) *.o
	$(RM) *.a