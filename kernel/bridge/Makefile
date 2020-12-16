include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: bridge.a

bridge.a: bridge.o 
	$(AR) $(ARFLAGS) $@ $^
	
bridge.o: bridge.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a