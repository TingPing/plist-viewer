makensis_verbose = $(makensis_verbose_$(V))
makensis_verbose_ = $(makensis_verbose_$(AM_DEFAULT_VERBOSITY))
makensis_verbose_0 = @echo "  NSIS    " $@; $(MAKENSIS) -DOUT_FILE=$@ $< > /dev/null;
makensis_verbose_1 = $(MAKENSIS) -DOUT_FILE=$@ $<;

installer = $(top_srcdir)/plist-viewer-$(VERSION).exe

$(installer): win32-installer.nsi
	$(makensis_verbose)

if HOST_WINDOWS
dist-hook: $(installer)
endif

EXTRA_DIST = win32-installer.nsi
