include ../../mk/build.mk

EXTRA_CFLAGS= -I..

all: devs.a 

# this finds the names of all subdirectories under this one, that contain a Makefile
SUBDIRS=$(shell find ./* -mindepth 1 -type f -name 'Makefile' | awk -F '/' '{print $$2}')
$(info $$SUBDIRS is [${SUBDIRS}])

ARCHIVE_DIRS=$(addsuffix /, $(SUBDIRS))
$(info $$ARCHIVE_DIRS is [${ARCHIVE_DIRS}])

ARCHIVE_NAMES=$(addsuffix .a, $(SUBDIRS))
$(info $$ARCHIVE_NAMES is [${ARCHIVE_NAMES}])

ARCHIVES=$(join $(ARCHIVE_DIRS),$(ARCHIVE_NAMES))
$(info $$ARCHIVES is [${ARCHIVES}])

devs.a: dev.o $(ARCHIVES)
	$(AR) $(ARFLAGS)T $@ dev.o $(ARCHIVES)
	
dev.o: dev.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

$(ARCHIVES):
	$(foreach file, $(SUBDIRS), cd $(file) && make && cd ..;)


clean: clean-subprojects
	$(RM) devs.a
	$(RM) dev.o

clean-subprojects:
	$(foreach file, $(SUBDIRS), cd $(file) && make clean && cd ..;)
