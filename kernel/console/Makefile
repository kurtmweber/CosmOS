include ../../mk/build.mk

EXTRA_CFLAGS= -I..
ARFLAGS=-crsT

all: drivers-dir console.a

console.a: kprintf.o drivers/drivers.a
	$(AR) $(ARFLAGS) $@ $^

kprintf.o: kprintf.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
	
drivers-dir:
	cd drivers && $(MAKE)
	
clean: drivers-dir-clean console-a-clean kprintf-o-clean

drivers-dir-clean:
	cd drivers && $(MAKE) clean

console-a-clean:
	$(RM) console.a
	
kprintf-o-clean:
	$(RM) kprintf.o