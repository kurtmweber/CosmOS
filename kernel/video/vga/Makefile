NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I../..
AR=ar
ARFLAGS=-crs
MAKE=make

all: vga.a

vga.a: cursor.o mode.o text.o vga.o
	$(AR) $(ARFLAGS) $@ $^
	
cursor.o: cursor.c
	$(CC) $(CFLAGS) -o $@ $^

mode.o: mode.c
	$(CC) $(CFLAGS) -o $@ $^
	
text.o: text.c
	$(CC) $(CFLAGS) -o $@ $^
	
vga.o: vga.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: vga-a-clean cursor-o-clean mode-o-clean text-o-clean vga-o-clean

vga-a-clean:
	$(RM) vga.a

cursor-o-clean:
	$(RM) cursor.o
	
mode-o-clean:
	$(RM) mode.o
	
text-o-clean:
	$(RM) text.o
	
vga-o-clean:
	$(RM) vga.o