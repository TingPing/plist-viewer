bin_PROGRAMS = plist-viewer
plist_viewer_SOURCES = main.c app.c window.c resources.c
plist_viewer_CFLAGS = $(GLIB_CFLAGS) $(GTK_CFLAGS) $(LIBPLIST_CFLAGS)
plist_viewer_LDADD = $(GLIB_LIBS) $(GTK_LIBS) $(LIBPLIST_LIBS)

resources.c: $(top_srcdir)/data/plist-viewer.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(top_srcdir)/data --generate-dependencies $(top_srcdir)/data/plist-viewer.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(top_srcdir)/data --generate-source $<

BUILT_SOURCES = resources.c
CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = window.h app.h
