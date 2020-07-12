NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: console.a

console.a: console.o consoleprint.o consoleutil.o cursor.o keyboard.o
	$(AR) $(AFLAGS) $@ $^

console.o: console.c
	$(CC) $(CFLAGS) -o $@ $^
	
consoleprint.o: consoleprint.c
	$(CC) $(CFLAGS) -o $@ $^
	
consoleutil.o: consoleutil.c
	$(CC) $(CFLAGS) -o $@ $^

cursor.o: cursor.c
	$(CC) $(CFLAGS) -o $@ $^
	
keyboard.o: keyboard.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: console-a-clean console-o-clean consoleprint-o-clean consoleutil-o-clean cursor-o-clean keyboard-o-clean

console-a-clean:
	$(RM) console.a

console-o-clean:
	$(RM) console.o
	
consoleprint-o-clean:
	$(RM) consoleprint.o
	
consoleutil-o-clean:
	$(RM) consoleutil.o

cursor-o-clean:
	$(RM) cursor.o
	
keyboard-o-clean:
	$(RM) keyboard.o