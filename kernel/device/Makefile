include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: device.a

device.a: device.o 
	$(AR) $(ARFLAGS) $@ $^
	
device.o: device.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a