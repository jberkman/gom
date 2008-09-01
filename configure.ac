#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
# http://www.mail-archive.com/autoconf@gnu.org/msg16720.html
AC_INIT([gom],
	[0.3.]m4_esyscmd([svnversion -n]),
	[http://code.google.com/p/ilovegom/issues/entry])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([xulapp/components/gom/src/xgGomModule.cpp])
AC_CONFIG_HEADER([config.h])

AC_ARG_WITH([libxul-sdk],
	[AS_HELP_STRING([--with-libxul-sdk],
		[path to XULlRunner SDK directory])],
	[XULSDK=$withval])
if test -z "$XULSDK" ; then
	AC_MSG_ERROR([You must specify the path to a XULRunner SDK with the --with-libxul-sdk option.])
fi
AC_SUBST(XULSDK)

AC_ARG_WITH([mozilla],
	[AS_HELP_STRING([--with-mozilla],
		[path to Mozilla source tree])],
	[MOZILLA=$withval])
if test -z "$MOZILLA" ; then
   	AC_MSG_ERROR([You must specify the path to a Mozilla source tree with the --mozilla option.])
fi
AC_SUBST(MOZILLA)

AC_MSG_CHECKING([additional mozconfig])
if test "$MOZCONFIG" ; then
   	AC_MSG_RESULT([$MOZCONFIG])
else
	AC_MSG_RESULT(none)
fi
AC_SUBST(MOZCONFIG)
AM_CONDITIONAL([HAVE_MOZCONFIG],[test x$MOZCONFIG != x])

AC_SUBST(DISTCHECK_CONFIGURE_FLAGS,["--with-libxul-sdk=$XULSDK --with-mozilla=$MOZILLA MOZCONFIG=$MOZCONFIG"])

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Checks for packages
GLIB_REQVER="2.6.0"
GTK_REQVER="2.6.0"

PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= $GTK_REQVER])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

has_check="false"
TEST_MODULES="$GOM_MODULES check >= 0.9.5"
PKG_CHECK_MODULES(TEST, [$TEST_MODULES], [has_check="true"], [AC_MSG_WARN([Check framework not found; make check disabled])])
AC_SUBST(TEST_CFLAGS)
AC_SUBST(TEST_LIBS)
AM_CONDITIONAL([HAVE_CHECK],[test x$has_check = xtrue])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LN_S
# AC_PROG_RANLIB
# AC_PROG_LIBTOOL
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

rm -rf a.out.dSYM

AC_CONFIG_FILES([
Makefile
xulapp/app/application.ini
xulapp/confvars.sh
xulapp/mozconfig
])

AC_CONFIG_COMMANDS([configure-mozilla],
	[$ac_top_srcdir/xulapp/configure-mozilla "$MOZILLA" "$ac_top_srcdir"],
	[MOZILLA=$MOZILLA])

AC_OUTPUT
