include ../../../mk/build.mk

EXTRA_CFLAGS=-I../..
ARFLAGS=-crsT

all: vga-dir serial-dir drivers.a

drivers.a: driverinterface.o vga/vga.a serial/serial_console.a
	$(AR) $(ARFLAGS) $@ $^

driverinterface.o: driverinterface.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
vga-dir:
	cd vga && $(MAKE)

serial-dir:
	cd serial && $(MAKE)

clean: vga-dir-clean serial-dir-clean
	$(RM) *.o
	$(RM) *.a
	
vga-dir-clean:
	cd vga && $(MAKE) clean

serial-dir-clean:
	cd serial && $(MAKE) clean