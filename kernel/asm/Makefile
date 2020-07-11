NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: asm.a

asm.a: misc.o
	$(AR) $(AFLAGS) asm.a misc.o
	
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