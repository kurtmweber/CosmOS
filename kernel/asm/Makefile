include ../../mk/build.mk

ARFLAGS=-crs
EXTRA_CFLAGS=-I..

all: asm.a

asm.a: misc.o
	$(AR) $(ARFLAGS) asm.a misc.o
	
#io.o: io.c
#	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
misc.o: misc.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean: asm-a-clean io-o-clean misc-o-clean

asm-a-clean:
	$(RM) asm.a
	
io-o-clean:
	$(RM) io.o
	
misc-o-clean:
	$(RM) misc.o