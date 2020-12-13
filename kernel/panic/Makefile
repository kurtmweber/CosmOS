include ../../mk/build.mk

CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
ARFLAGS=-crs
MAKE=make

all: panic.a

panic.a: panic.o
	$(AR) $(ARFLAGS) $@ $^
	
panic.o: panic.c panic.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: panic-a-clean panic-o-clean

panic-a-clean:
	$(RM) panic.a
	
panic-o-clean:
	$(RM) panic.o