NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: ata.a

ata.a: ata.o control.o registers.o
	$(AR) $(AFLAGS) $@ $^
	
ata.o: ata.c
	$(CC) $(CFLAGS) -o $@ $^
	
control.o: control.c
	$(CC) $(CFLAGS) -o $@ $^

registers.o: registers.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: ata-a-clean ata-o-clean control-o-clean registers-o-clean

ata-a-clean:
	$(RM) ata.a

ata-o-clean:
	$(RM) ata.o

control-o-clean:
	$(RM) control.o
	
registers-o-clean:
	$(RM) registers.o