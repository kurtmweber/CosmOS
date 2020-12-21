include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: devicemgr.a

devicemgr.a: devicemgr.o devicetypes.o deviceregistry.o
	$(AR) $(ARFLAGS) $@ $^
	
devicemgr.o: devicemgr.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^
	
devicetypes.o: devicetypes.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

deviceregistry.o: deviceregistry.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a