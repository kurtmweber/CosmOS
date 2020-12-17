include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: pci.a

pci.a: pci_address.o pci_header.o pci_init.o pci_scan.o pci_search.o
	$(AR) $(ARFLAGS) $@ $^
	
pci_address.o: pci_address.c pci.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
pci_header.o: pci_header.c pci.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
pci_init.o: pci_init.c pci.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
pci_scan.o: pci_scan.c pci.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

pci_search.o: pci_search.c pci.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean:
	$(RM) *.o
	$(RM) *.a