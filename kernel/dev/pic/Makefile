include ../../../mk/build.mk

EXTRA_CFLAGS= -I../.. -DRTC_SLEEP
ARFLAGS=-crs

all: pic.a

pic.a: pic.o
	$(AR) $(ARFLAGS) $@ $^
	
pic.o: pic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a