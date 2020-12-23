include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: virtio.a

virtio.a: vnic.o vblock.o virtqueue.o virtio.o
	$(AR) $(ARFLAGS) $@ $^

vnic.o: vnic/vnic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

vblock.o: vblock/vblock.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

virtqueue.o: virtqueue.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

virtio.o: virtio.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a