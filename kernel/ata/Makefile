NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: ata.a

ata.a: ata.o
	$(AR) $(AFLAGS) $@ $^
	
ata.o: ata.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: ata-a-clean ata-o-clean

ata-a-clean:
	$(RM) ata.a

ata-o-clean:
	$(RM) ata.o