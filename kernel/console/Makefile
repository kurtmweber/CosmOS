include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crsT

archive: drivers-dir console.a

console.a: kprintf.o drivers/drivers.a
	$(AR) $(ARFLAGS) $@ $^

kprintf.o: kprintf.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
drivers-dir:
	cd drivers && $(MAKE)
	
clean: drivers-dir-clean
	$(RM) *.o
	$(RM) *.a	
	
drivers-dir-clean:
	cd drivers && $(MAKE) clean
