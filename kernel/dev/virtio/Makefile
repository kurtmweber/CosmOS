include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: virtio.a

virtio.a: vnic.o vblock.o virtqueue.o virtio.o
	$(AR) $(ARFLAGS) $@ $^

vnic.o: vnic/vnic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

vblock.o: vblock/vblock.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

virtqueue.o: virtqueue.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

virtio.o: virtio.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a