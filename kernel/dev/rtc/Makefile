include ../../../mk/build.mk

EXTRA_CFLAGS= -I../.. -DRTC_SLEEP
ARFLAGS=-crs

all: rtc.a

rtc.a: rtc.o
	$(AR) $(ARFLAGS) $@ $^
	
rtc.o: rtc.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a