include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: collection.a

collection.a: array.o
	$(AR) $(ARFLAGS) $@ $^
	
array.o: array/array.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a