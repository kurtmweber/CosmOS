include ../../../mk/build.mk

ARCHIVE_FILE=system_platform.a

archive: $(ARCHIVE_FILE)

ifeq ($(TARGET_PLATFORM),TARGET_PLATFORM_ARM)
TARGETDIR=arm
else
TARGETDIR=i386
endif

$(ARCHIVE_FILE):
	cd $(TARGETDIR) && make archive && cd ..;
	$(AR) $(ARFLAGS) $(ARCHIVE_FILE) $(TARGETDIR)/$(TARGETDIR).a

clean:
	cd $(TARGETDIR) && make clean && cd ..;
	$(RM) *.a

EXTRA_CFLAGS= -I..

