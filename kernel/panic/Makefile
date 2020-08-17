NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: panic.a

panic.a: panic.o
	$(AR) $(AFLAGS) $@ $^
	
panic.o: panic.c panic.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: panic-a-clean panic-o-clean

panic-a-clean:
	$(RM) panic.a
	
panic-o-clean:
	$(RM) panic.o