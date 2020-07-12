NASM=nasm -f bin
RM=rm -f
CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
AR=ar
AFLAGS=-crs
MAKE=make

all: timing.a

timing.a: rtc.o
	$(AR) $(AFLAGS) $@ $^
	
rtc.o: rtc.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: timing-a-clean rtc-o-clean

timing-a-clean:
	$(RM) timing.a

rtc-o-clean:
	$(RM) rtc.o