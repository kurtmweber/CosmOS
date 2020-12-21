include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: usb.a

usb.a: usb.o 
	$(AR) $(ARFLAGS) $@ $^
	
usb.o: usb.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a