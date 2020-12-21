include ../../../mk/build.mk

EXTRA_CFLAGS= -I../.. -DRTC_SLEEP
ARFLAGS=-crs

all: pit.a

pit.a: pit.o
	$(AR) $(ARFLAGS) $@ $^
	
pit.o: pit.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a