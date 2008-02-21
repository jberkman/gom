#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([gom], [0.0], [http://code.google.com/p/loolixbodes/issues/list])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/gom/gom.c])
AC_CONFIG_HEADER([config.h])

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Checks for packages
GTK_REQVER="2.6.0"

PKG_CHECK_MODULES(GOM, gtk+-2.0 >= $GTK_REQVER, gthread-2.0)
AC_SUBST(GOM_CFLAGS)
AC_SUBST(GOM_LIBS)

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_LIBTOOL

# Make libtool use --silent when --silent is passed to make
changequote(,)dnl
LIBTOOL="${LIBTOOL} \$(shell echo \"\$(MFLAGS)\" | awk '/^[^ ]*s/ { print \"--silent\" }')"
changequote([,])dnl

# Checks for programs.

# Checks for libraries.
AC_ARG_WITH([mozilla-source],
	[AS_HELP_STRING([--with-mozilla-source],
		[path to checked-out and built Mozilla JavaScript source tree])],
	[mozilla_source=$withval])
echo "mozilla_source: $mozilla_source"
if test -z "$mozilla_source" ; then
	AC_MSG_ERROR([You must specify the path to a checked-out and built Mozilla JavaScript source tree with the --with-mozilla-source option.])
fi
AC_MSG_CHECKING([for jsapi.c in $mozilla_source])
if test -r "$mozilla_source"/jsapi.c ; then
	AC_MSG_RESULT([found])
else
	AC_MSG_RESULT([not found])
	AC_MSG_CHECKING([for jsapi.c in $mozilla_source/js/src])
	if test -r "$mozilla_source"/js/src/jsapi.c ; then
		AC_MSG_RESULT([found])
		mozilla_source="$mozilla_source"/js/src
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([$mozilla_source does not look like a Mozilla JavaScript source tree.])
	fi
fi
AC_MSG_CHECKING([for config.mk in $mozilla_source])
if test -r "$mozilla_source"/config.mk ; then
	AC_MSG_RESULT([found])
else
	AC_MSG_RESULT([not found])
	AC_MSG_ERROR([$mozilla_source does not look like a Mozilla JavaScript source tree.])
fi
mozmake="make -s mozilla_source=$mozilla_source -f $srcdir/Makefile.configure"
AC_MSG_CHECKING([for Mozilla OBJDIR])
moz_objdir=$($mozmake objdir)
AC_MSG_RESULT([$moz_objdir])
AC_MSG_CHECKING([for libjs.a in $mozilla_source/$moz_objdir])
if test -r "$mozilla_source/$moz_objdir"/libjs.a ; then
	AC_MSG_RESULT([found])
else
	AC_MSG_RESULT([not found])
	AC_MSG_ERROR([$mozilla_source does not look like a Mozilla JavaScript source tree.])
fi
moz_oscflags=$($mozmake oscflags)
moz_oslibs=$($mozmake oslibs)
AC_SUBST(JS_CFLAGS, ["$moz_oscflags -I$mozilla_source -I$mozilla_source/$moz_objdir"])
AC_SUBST(JS_LIBS, ["$mozilla_source/$moz_objdir/libjs.a $moz_oslibs"])
AC_SUBST(DISTCHECK_CONFIGURE_FLAGS,["--with-mozilla-source=$mozilla_source"])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

rm -rf a.out.dSYM

AC_CONFIG_FILES([
gom-0.pc
Makefile
])
AC_OUTPUT
