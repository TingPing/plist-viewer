AC_PREREQ([2.69])
AC_INIT([plist-viewer], [0.1.0])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC_C99
AC_PROG_INSTALL

IT_PROG_INTLTOOL([0.40.0])

AM_INIT_AUTOMAKE([1.11 silent-rules tar-pax dist-xz no-dist-gzip subdir-objects no-define foreign -Wall])
AM_SILENT_RULES([yes])
AM_PATH_GLIB_2_0([2.40.0], , AC_MSG_ERROR(glib not found!), [gmodule gobject gio])
AM_GLIB_GNU_GETTEXT

PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.10])
PKG_CHECK_MODULES([LIBPLIST], [libplist])


#if test "$CC" = "clang"; then
#    AC_SUBST([CFLAGS], ["$CFLAGS -Weverything -Wno-documentation -Wno-padded -Wno-unused-parameter -Wno-used-but-marked-unused"])
if test "$CC" = "gcc" || test "$CC" = "clang"; then
	AC_SUBST([CFLAGS], ["$CFLAGS -Wall -Wextra -Wno-unused-parameter"])
fi

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    data/Makefile
    po/Makefile.in
])

AC_OUTPUT
echo "
    $PACKAGE $VERSION
    
    prefix: ${prefix}
"