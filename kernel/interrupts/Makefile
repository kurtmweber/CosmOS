include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ISRCFLAGS=-mgeneral-regs-only
ARFLAGS=-crs

all: interrupts.a

interrupts.a: de.o idt.o irq.o pic.o interrupt_router.o
	$(AR) $(ARFLAGS) interrupts.a de.o idt.o irq.o pic.o interrupt_router.o
	
de.o: de.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $<

idt.o: idt.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
irq.o: irq.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $<
	
pic.o: pic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
interrupt_router.o: interrupt_router.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

clean:
	$(RM) *.o
	$(RM) *.a