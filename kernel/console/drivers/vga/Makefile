NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I../../..
AR=ar
ARFLAGS=-crs
MAKE=make

all: vga.a

vga.a: vga_console.o vga_console_write.o
	$(AR) $(ARFLAGS) $@ $^
	
vga_console.o: vga_console.c
	$(CC) $(CFLAGS) -o $@ $^

vga_console_write.o: vga_console_write.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: vga-a-clean vga_console-o-clean vga_console_write-o-clean

vga_console-o-clean:
	$(RM) vga_console.o
	
vga_console_write-o-clean:
	$(RM) vga_console_write.o
	
vga-a-clean:
	$(RM) vga.a