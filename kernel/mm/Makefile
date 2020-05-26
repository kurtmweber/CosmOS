NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: mm.a

mm.a: blockmgmt.o init.o kmalloc.o map.o pagetables.o
	$(AR) $(AFLAGS) $@ $?
	
blockmgmt.o: blockmgmt.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
init.o: init.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
kmalloc.o: kmalloc.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
map.o: map.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
pagetables.o: pagetables.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: mm-a-clean blockmgmt-o-clean init-o-clean map-o-clean pagetables-o-clean

mm-a-clean:
	$(RM) console.a
	
blockmgmt-o-clean:
	$(RM) blockmgmt.o
	
init-o-clean:
	$(RM) init.o

map-o-clean:
	$(RM) map.o
	
pagetables-o-clean:
	$(RM) pagetables.o