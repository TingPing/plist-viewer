mimedir = $(datadir)/mime/packages
mime_DATA = plist-viewer.xml

appdatadir = $(datadir)/appdata
appdata_in_files = plist-viewer.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

desktopdir = $(datadir)/applications
desktop_in_files = plist-viewer.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@


UPDATE_MIME_DATABASE = update-mime-database "$(datadir)/mime" || :
UPDATE_DESKTOP_DATABASE = update-desktop-database -q "$(desktopdir)" || :

install-data-hook:
	$(UPDATE_MIME_DATABASE);
	$(UPDATE_DESKTOP_DATABASE);

uninstall-hook:
	$(UPDATE_MIME_DATABASE);
	$(UPDATE_DESKTOP_DATABASE);

CLEANFILES = $(appdata_DATA) $(desktop_DATA)
EXTRA_DIST = ui gtk $(appdata_in_files) $(desktop_in_files) $(mime_DATA) plist-viewer.gresource.xml
