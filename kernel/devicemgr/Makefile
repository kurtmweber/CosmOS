include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: devicemgr.a

devicemgr.a: devicemgr.o 
	$(AR) $(ARFLAGS) $@ $^
	
devicemgr.o: devicemgr.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a