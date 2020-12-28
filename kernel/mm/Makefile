include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crs

all: mm.a

mm.a: blockmgmt.o directmap.o init.o kmalloc.o map.o pagetables.o phys_alloc.o mm.o
	$(AR) $(ARFLAGS) $@ $?
	
blockmgmt.o: blockmgmt.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

directmap.o: directmap.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
init.o: init.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
kmalloc.o: kmalloc.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
map.o: map.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
pagetables.o: pagetables.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

phys_alloc.o: phys_alloc.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
mm.o: mm.c mm.h
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
clean:
	$(RM) *.o
	$(RM) *.a