include ../../mk/build.mk

ARFLAGS=-crs
EXTRA_CFLAGS=-I..

all: asm.a

asm.a: misc.o io.o
	$(AR) $(ARFLAGS) asm.a misc.o io.o
	
io.o: io.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
misc.o: misc.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean: 
	$(RM) *.o
	$(RM) *.a
