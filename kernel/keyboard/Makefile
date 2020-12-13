include ../../mk/build.mk

CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ISRCFLAGS=-mgeneral-regs-only
AR=ar
ARFLAGS=-crs
MAKE=make

all: keyboard.a

keyboard.a: keyboard.o keyboardbuffer.o
	$(AR) $(ARFLAGS) $@ $^
	
keyboard.o: keyboard.c
	$(CC) $(CFLAGS) $(ISRCFLAGS) -o $@ $^
	
keyboardbuffer.o: keyboardbuffer.c
	$(CC) $(CFLAGS) $(ISRCFLAGS) -o $@ $^
	
clean: keyboard-a-clean keyboard-o-clean keyboardbuffer-o-clean

keyboard-a-clean:
	$(RM) keyboard.a

keyboard-o-clean:
	$(RM) keyboard.o
	
keyboardbuffer-o-clean:
	$(RM) keyboardbuffer.o