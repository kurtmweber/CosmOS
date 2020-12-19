include ../../mk/build.mk

EXTRA_CFLAGS= -I.. -DRTC_SLEEP
ARFLAGS=-crs

all: sleep.a

sleep.a: sleep.o
	$(AR) $(ARFLAGS) $@ $^
	
sleep.o: sleep.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a