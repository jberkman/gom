#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([gom], [0.3], [http://code.google.com/p/ilovegom/issues/entry])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/gom/gom.c])
AC_CONFIG_HEADER([config.h])

AC_ARG_WITH([xulrunner-sdk],
	[AS_HELP_STRING([--with-xulrunner-sdk],
		[path to XulRunner SDK directory])],
	[xulrunnersdk=$withval])
if test -z "$xulrunnersdk" ; then
	AC_MSG_ERROR([You must specify the path to a XulRunner SDK with the --with-xulrunner-sdk option.])
fi
AC_SUBST(DISTCHECK_CONFIGURE_FLAGS,["--with-xulrunner-sdk=$xulrunnersdk"])

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Checks for packages
GLIB_REQVER="2.6.0"
GTK_REQVER="2.6.0"
CURL_REQVER="0"

GOM_MODULES="gtk+-2.0 >= $GTK_REQVER gthread-2.0"
PKG_CHECK_MODULES(GOM, [$GOM_MODULES])
AC_SUBST(GOM_CFLAGS)
AC_SUBST(GOM_LIBS)

has_check="false"
TEST_MODULES="$GOM_MODULES check >= 0.9.5"
PKG_CHECK_MODULES(TEST, [$TEST_MODULES], [has_check="true"], [AC_MSG_WARN([Check framework not found; make check disabled])])
AC_SUBST(TEST_CFLAGS)
AC_SUBST(TEST_LIBS)
AM_CONDITIONAL([HAVE_CHECK],[test x$has_check = xtrue])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_RANLIB

# Make libtool use --silent when --silent is passed to make
changequote(,)dnl
LIBTOOL="${LIBTOOL} \$(shell echo \"\$(MFLAGS)\" | awk '/^[^ ]*s/ { print \"--silent\" }')"
changequote([,])dnl

# Checks for libraries.
#
# Run AM_PATH_GLIB_2_0 to make sure that GLib is installed and working
# 

GLIB_PACKAGES="gobject-2.0 gmodule-no-export-2.0"

AM_PATH_GLIB_2_0(["$GLIB_REQVER"], :,
  AC_MSG_ERROR([
*** GLIB ["$GLIB_REQVER"] or better is required. The latest version of 
*** GLIB is always available from ftp://ftp.gtk.org/pub/gtk/.]),
  gobject gmodule-no-export gthread)

# Checks for header files.
CPPFLAGS_save=$CPPFLAGS

js_cppflags="-I$xulrunnersdk/include/js -DXP_UNIX"
CPPFLAGS="$CPPFLAGS_save $js_cppflags"
AC_CHECK_HEADERS([jsapi.h],[],[
	AC_MSG_ERROR([$xulrunnersdk is missing Spidermonkey headers.])
])
GOM_CFLAGS="$GOM_CFLAGS $js_cppflags"

xulrunner_cppflags="-I$xulrunnersdk/sdk/include -DXPCOMGLUE=1"
CPPFLAGS="$CPPFLAGS_save $xulrunner_cppflags"
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([nsXPCOM.h nsIIOService.h],[],[
	AC_MSG_ERROR([$xulrunnersdk is missing XPCOM headers.])
])
AC_LANG_POP([C++])
GOM_CFLAGS="$GOM_CFLAGS $xulrunner_cppflags"

CPPFLAGS=$CPPFLAGS_save

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
LIBS_save=$LIBS

xulrunner_libs="-L$xulrunnersdk/sdk/lib"
LIBS="$LIBS_save $xulrunner_libs"
AC_CHECK_LIB([mozjs],[JS_Init],[],[
	AC_MSG_ERROR([$xulrunner is missing Spidermonkey libraries.])
])
AC_CHECK_LIB([xpcomglue],[XPCOMGlueStartup],[],[
	AC_MSG_ERROR([$xulrunnersdk is missing XPCOM libraries.])
])
GOM_LIBS="$GOM_LIBS $xulrunner_libs -lxpcomglue -lmozjs"

LIBS=$LIBS_save

rm -rf a.out.dSYM

AC_CONFIG_FILES([
gom-0.pc
Makefile
])
AC_OUTPUT
