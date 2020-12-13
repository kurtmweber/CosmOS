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
	
clean: vga-dir-clean drivers-a-clean driverinterface-o-clean

drivers-a-clean:
	$(RM) drivers.a

driverinterface-o-clean:
	$(RM) driverinterface.o
	
vga-dir-clean:
	cd vga && $(MAKE) clean