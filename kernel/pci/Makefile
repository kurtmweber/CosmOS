include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crs

all: pci.a

pci.a: pci_address.o pci_header.o pci_init.o pci_scan.o
	$(AR) $(ARFLAGS) $@ $^
	
pci_address.o: pci_address.c pci.h
	$(CC) $(CFLAGS) -o $@ $<
	
pci_header.o: pci_header.c pci.h
	$(CC) $(CFLAGS) -o $@ $<
	
pci_init.o: pci_init.c pci.h
	$(CC) $(CFLAGS) -o $@ $<
	
pci_scan.o: pci_scan.c pci.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: pci-a-clean pci_address-o-clean pci_init-o-clean

pci-a-clean:
	$(RM) pci.a
	
pci_address-o-clean:
	$(RM) pci_address.o
	
pci_header-o-clean:
	$(RM) pci_header.o
	
pci_init-o-clean:
	$(RM) pci_init.o