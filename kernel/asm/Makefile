include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crs

all: asm.a

asm.a: misc.o
	$(AR) $(ARFLAGS) asm.a misc.o
	
#io.o: io.c
#	$(CC) $(CFLAGS) -o $@ $<
	
misc.o: misc.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean: asm-a-clean io-o-clean misc-o-clean

asm-a-clean:
	$(RM) asm.a
	
io-o-clean:
	$(RM) io.o
	
misc-o-clean:
	$(RM) misc.o