include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: bridge.a

bridge.a: bridge.o inteli440fx.o intel82371ab.o intelpciisa.o
	$(AR) $(ARFLAGS) $@ $^
	
bridge.o: bridge.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

inteli440fx.o: inteli440fx/inteli440fx.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

intel82371ab.o: intel82371ab/intel82371ab.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

intelpciisa.o: intelpciisa/intelpciisa.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *.a