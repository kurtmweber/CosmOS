include ../../mk/build.mk

CFLAGS=-c -masm=intel -m64 -mno-red-zone -ffreestanding -fno-pic -O0 -mcmodel=large -I..
ARFLAGS=-crs

all: mm.a

mm.a: blockmgmt.o init.o kmalloc.o map.o pagetables.o phys_alloc.o
	$(AR) $(ARFLAGS) $@ $?
	
blockmgmt.o: blockmgmt.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
init.o: init.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
kmalloc.o: kmalloc.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
map.o: map.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
pagetables.o: pagetables.c mm.h
	$(CC) $(CFLAGS) -o $@ $<

phys_alloc.o: phys_alloc.c mm.h
	$(CC) $(CFLAGS) -o $@ $<
	
clean: mm-a-clean blockmgmt-o-clean init-o-clean kmalloc-o-clean map-o-clean pagetables-o-clean phys_alloc-o-clean

mm-a-clean:
	$(RM) mm.a
	
blockmgmt-o-clean:
	$(RM) blockmgmt.o
	
init-o-clean:
	$(RM) init.o
	
kmalloc-o-clean:
	$(RM) kmalloc.o

map-o-clean:
	$(RM) map.o
	
pagetables-o-clean:
	$(RM) pagetables.o

phys_alloc-o-clean:
	$(RM) phys_alloc.o