NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: console.a

console.a: video.o printk.o
	$(AR) $(AFLAGS) console.a video.o
	
printk.o: printk.c
	$(CC) $(CFLAGS) -o printk.o printk.c

video.o: video.c
	$(CC) $(CFLAGS) -o video.o video.c
	
clean: console-a-clean printk-o-clean video-o-clean

console-a-clean:
	$(RM) console.a
	
printk-o-clean:
	$(RM) printk.o

video-o-clean:
	$(RM) video.o