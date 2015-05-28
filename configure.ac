AC_PREREQ([2.69])
AC_INIT([plist-viewer], [0.1.1])
AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PATH_PROG([MAKENSIS], [makensis], [no])
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AM_INIT_AUTOMAKE([1.11 silent-rules tar-pax dist-xz no-dist-gzip subdir-objects no-define foreign -Wall])
AM_SILENT_RULES([yes])

GETTEXT_PACKAGE="plist-viewer"
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "$GETTEXT_PACKAGE", [Gettext package name])
IT_PROG_INTLTOOL([0.40.0])
AM_GLIB_GNU_GETTEXT

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.40 gio-2.0])
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.10])
PKG_CHECK_MODULES([LIBPLIST], [libplist])

CFLAGS="$CFLAGS"
HOST_WINDOWS=no

AS_CASE([$host_os],
	[*mingw*], [
		CFLAGS="$CFLAGS -mwindows"
		HOST_WINDOWS=yes
		if test "$MAKESIS" = "no"; then
			AC_MSG_WARN(makensis required for make dist)
		fi
	]
)

AX_APPEND_COMPILE_FLAGS([ \
	-Wall \
	-Wextra \
	-Wno-padded \
	-Wno-unused-parameter \
	-Wstrict-prototypes \
	-Werror=missing-prototypes \
	-Werror=implicit-function-declaration \
	-Werror=pointer-arith \
	-Werror=init-self \
	-Werror=format-security \
	-Werror=format=2 \
	-Werror=missing-include-dirs \
])

AM_CONDITIONAL(HOST_WINDOWS, test "$HOST_WINDOWS" = "yes")
AC_SUBST([CFLAGS])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    data/Makefile
    dist/Makefile
    po/Makefile.in
])

AC_OUTPUT
echo "
    $PACKAGE $VERSION

    prefix: ${prefix}
"
