include ../../mk/build.mk

CC=gcc
CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I.. -DRTC_SLEEP
AR=ar
ARFLAGS=-crs
MAKE=make

all: timing.a

timing.a: rtc.o sleep.o
	$(AR) $(ARFLAGS) $@ $^
	
rtc.o: rtc.c
	$(CC) $(CFLAGS) -o $@ $^
	
sleep.o: sleep.c
	$(CC) $(CFLAGS) -o $@ $^
	
clean: timing-a-clean rtc-o-clean sleep-o-clean

timing-a-clean:
	$(RM) timing.a

rtc-o-clean:
	$(RM) rtc.o
	
sleep-o-clean:
	$(RM) sleep.o