NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: debug_error.a

debug_error.a: debugint.o panic.o
	$(AR) $(AFLAGS) $@ $^
	
debugint.o: debugint.c debug_error.h
	$(CC) $(CFLAGS) -o $@ $<
	
panic.o: panic.c debug_error.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: debug_error-a-clean debugint-o-clean panic-o-clean

debug_error-a-clean:
	$(RM) debug_error.a

debugint-o-clean:
	$(RM) debugint.o
	
panic-o-clean:
	$(RM) panic.o