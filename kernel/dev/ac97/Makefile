include ../../../mk/build.mk

EXTRA_CFLAGS= -I../.. -DRTC_SLEEP
ARFLAGS=-crs

all: ac97.a

ac97.a: ac97.o
	$(AR) $(ARFLAGS) $@ $^
	
ac97.o: ac97.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^
	
clean:
	$(RM) *.o
	$(RM) *.a