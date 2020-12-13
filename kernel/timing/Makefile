include ../../mk/build.mk

EXTRA_CFLAGS= -I.. -DRTC_SLEEP
ARFLAGS=-crs

all: timing.a

timing.a: rtc.o sleep.o
	$(AR) $(ARFLAGS) $@ $^
	
rtc.o: rtc.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
sleep.o: sleep.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean: timing-a-clean rtc-o-clean sleep-o-clean

timing-a-clean:
	$(RM) timing.a

rtc-o-clean:
	$(RM) rtc.o
	
sleep-o-clean:
	$(RM) sleep.o