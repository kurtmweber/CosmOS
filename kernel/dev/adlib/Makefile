include ../../../mk/build.mk

EXTRA_CFLAGS= -I../.. -DRTC_SLEEP
ARFLAGS=-crs

all: adlib.a

adlib.a: adlib.o
	$(AR) $(ARFLAGS) $@ $^
	
adlib.o: adlib.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a