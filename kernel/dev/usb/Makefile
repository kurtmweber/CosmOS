include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: usb.a

usb.a: usb_controller.o 
	$(AR) $(ARFLAGS) $@ $^
	
usb_controller.o: usb_controller.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a