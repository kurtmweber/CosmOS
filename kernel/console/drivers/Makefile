include ../../../mk/build.mk

EXTRA_CFLAGS=-I../..
ARFLAGS=-crsT

all: vga-dir drivers.a

drivers.a: driverinterface.o vga/vga.a
	$(AR) $(ARFLAGS) $@ $^

driverinterface.o: driverinterface.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
vga-dir:
	cd vga && $(MAKE)
	
clean: vga-dir-clean
	$(RM) *.o
	$(RM) *.a
	
vga-dir-clean:
	cd vga && $(MAKE) clean