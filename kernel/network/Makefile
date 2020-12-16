include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: network.a

network.a: network.o 
	$(AR) $(ARFLAGS) $@ $^
	
network.o: network.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a