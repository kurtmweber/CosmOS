NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: mm.a

mm.a: map.o
	$(AR) $(AFLAGS) $@ $?
	
map.o: map.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean: mm-a-clean map-o-clean

mm-a-clean: map-o-clean
	$(RM) console.a

map-o-clean:
	$(RM) map.o