NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
ARFLAGS=-crsT
MAKE=make

all: drivers-dir console.a

console.a: kprintf.o drivers/drivers.a
	$(AR) $(ARFLAGS) $@ $^

kprintf.o: kprintf.c
	$(CC) $(CFLAGS) -o $@ $<
	
drivers-dir:
	cd drivers && $(MAKE)
	
clean: drivers-dir-clean console-a-clean kprintf-o-clean

drivers-dir-clean:
	cd drivers && $(MAKE) clean

console-a-clean:
	$(RM) console.a
	
kprintf-o-clean:
	$(RM) kprintf.o