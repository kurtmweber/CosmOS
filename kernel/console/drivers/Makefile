NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I../..
AR=ar
AFLAGS=-crsT
MAKE=make

all: vga-dir drivers.a

drivers.a: driverinterface.o vga/vga.a
	$(AR) $(AFLAGS) $@ $^

driverinterface.o: driverinterface.c
	$(CC) $(CFLAGS) -o $@ $^
	
vga-dir:
	cd vga && $(MAKE)
	
clean: vga-dir-clean drivers-a-clean driverinterface-o-clean

drivers-a-clean:
	$(RM) drivers.a

driverinterface-o-clean:
	$(RM) driverinterface.o
	
vga-dir-clean:
	cd vga && $(MAKE) clean