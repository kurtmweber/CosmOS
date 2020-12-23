include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: virtio.a

virtio.a: vnic.o
	$(AR) $(ARFLAGS) $@ $^

vnic.o: vnic/vnic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a