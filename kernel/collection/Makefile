include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: collection.a

collection.a: array.o list.o ringbuffer.o
	$(AR) $(ARFLAGS) $@ $^
	
array.o: array/array.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

list.o: list/list.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

ringbuffer.o: ringbuffer/ringbuffer.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a