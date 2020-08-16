NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crsT
MAKE=make

all: drivers-dir console.a

console.a: drivers/drivers.a
	$(AR) $(AFLAGS) $@ $^
	
drivers-dir:
	cd drivers && $(MAKE)
	
clean: drivers-dir-clean console-a-clean

drivers-dir-clean:
	cd drivers && $(MAKE) clean

console-a-clean:
	$(RM) console.a