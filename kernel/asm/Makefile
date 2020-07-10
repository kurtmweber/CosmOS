NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
MAKE=make

all: asm.o io.o

asm.o: asm.c
	$(CC) $(CFLAGS) -o $@ $<
	
io.o: io.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean: asm-o-clean io-o-clean

asm-o-clean:
	$(RM) asm.o
	
io-o-clean:
	$(RM) io.o