include ../../../mk/build.mk

EXTRA_CFLAGS= -I../..
ARFLAGS=-crs

all: network.a

network.a: ne2000pci.o rtl8139.o network.o e1000.o ne2000isa.o vnic.o
	$(AR) $(ARFLAGS) $@ $^

network.o: network.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

ne2000pci.o: ne2000pci/ne2000pci.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

ne2000isa.o: ne2000isa/ne2000isa.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

rtl8139.o: rtl8139/rtl8139.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

e1000.o: e1000/e1000.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

vnic.o: vnic/vnic.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(ISRCFLAGS) -o $@ $^

clean:
	$(RM) -r *.o
	$(RM) *.a