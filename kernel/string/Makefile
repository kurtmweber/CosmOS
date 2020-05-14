NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: string.a

string.a: itoa.o
	$(AR) $(AFLAGS) $@ $^
	
itoa.o: itoa.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: string-a-clean itoa-o-clean

string-a-clean:
	$(RM) string.a

itoa-o-clean:
	$(RM) itoa.o