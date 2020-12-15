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
	
clean:
	$(RM) *.o
	$(RM) *.a