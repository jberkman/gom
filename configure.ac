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
# AC_PROG_RANLIB
AC_PROG_LIBTOOL
# Make libtool use --silent when --silent is passed to make
changequote(,)dnl
LIBTOOL="${LIBTOOL} \$(shell echo \"\$(MFLAGS)\" | awk '/^[^ ]*s/ { print \"--silent\" }')"
changequote([,])dnl

AC_CHECK_PROG([XPIDL],[xpidl],[$xulrunnersdk/bin/xpidl],[],[$xulrunnersdk/bin])
if test -z "$XPIDL" ; then
   AC_MSG_ERROR([$xuldunnersdk is missing xpidl])
fi
XPIDL_FLAGS="-I $xulrunnersdk/idl"
AC_SUBST(XPIDL_FLAGS)

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

xulrunner_cppflags="-DDEBUG -include xpcom-config.h -I$xulrunnersdk/sdk/include"
#xulrunner_cppflags="-DMOZILLA_INTERNAL_API $xulrunner_cppflags"
for subdir in content xpcom dom xpconnect js widget layout ; do
    xulrunner_cppflags="$xulrunner_cppflags -I$xulrunnersdk/include/$subdir"
done
CPPFLAGS="$CPPFLAGS_save $xulrunner_cppflags"
CXXFLAGS="$CXXFLAGS -fshort-wchar -fno-rtti -fno-exceptions"
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([nsIXTFElement.h nsIAtom.h nsDOMError.h nsIXPCScriptable.h nsEvent.h nsCompatibility.h],[],[
	AC_MSG_ERROR([$xulrunnersdk is missing XPCOM headers.])
])
AC_LANG_POP([C++])
XPGOM_CFLAGS="$GOM_CFLAGS $xulrunner_cppflags"

CPPFLAGS=$CPPFLAGS_save

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
LIBS_save=$LIBS

# http://developer.mozilla.org/en/docs/XPCOM_Glue#Sample_Compiler.2FLinker_Flags
xulrunner_libs="-L$xulrunnersdk/sdk/lib"
LIBS="$LIBS_save $xulrunner_libs"
case $host_os in
     darwin*)
	xplibpath="-Wl,-executable_path,$xulrunnersdk/bin"
	;;
    linux*)
	xplibpath="-Wl,-rpath-link,$xulrunnersdk/bin"
	;;
    *)
	AC_MSG_ERROR([I don't know how to link components on $host_os])
	;;
esac
# "Internal Linkage" :(
#XPGOM_LIBS="$GOM_LIBS $xulrunner_libs -L$xulrunnersdk/lib $xulrunnersdk/../docshell/build/libdocshell.dylib -lxpcom_core -lxpcom -lnspr4"
# "Dependent Glue"
XPGOM_LIBS="$GOM_LIBS $xulrunner_libs -L$xulrunnersdk/bin $xplibpath -lxpcomglue_s -lxpcom -lnspr4"
# "Standalone Glue"
#XPGOM_LIBS="$GOM_LIBS $xulrunner_libs -lxpcomglue"

LIBS=$LIBS_save

rm -rf a.out.dSYM

AC_SUBST(XPGOM_CFLAGS)
AC_SUBST(XPGOM_LIBS)

AC_CONFIG_FILES([
gom-0.pc
Makefile
src/xulapp/application.ini
])
AC_OUTPUT
