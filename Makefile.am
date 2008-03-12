#
# This is a non-recursive automake file.
#
# The style was inspired by Robert Collins, as mentioned here:
#
#     http://sources.redhat.com/ml/automake/2001-08/msg00061.html
#
# And the original Recursive Make Considered Harmful:
#
#     http://www.pcug.org.au/~millerp/rmch/recu-make-cons-harm.html
#

AUTOMAKE_OPTIONS := subdir-objects
ACLOCAL_AMFLAGS =

NULL :=

# appsdir := $(datadir)/applications

bin_PROGRAMS := 
# sbin_PROGRAMS :=
noinst_PROGRAMS :=
check_PROGRAMS :=

# sbin_SCRIPTS :=

noinst_LTLIBRARIES :=

# dist_sbin_SCRIPTS :=
dist_noinst_SCRIPTS :=
# dist_pkglibexec_SCRIPTS :=

lib_LTLIBRARIES :=

# apps_DATA :=
dist_noinst_DATA := 
# nobase_dist_pkgdata_DATA :=
noinst_DATA :=

noinst_HEADERS :=

BUILT_SOURCES :=
CLEANFILES :=

TESTS :=

AM_CPPFLAGS :=

dist_noinst_DATA += Makefile.am.subdir Makefile.configure

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA := gom-0.pc

# pkglibexecdir = $(libexecdir)/$(PACKAGE)

SUBDIRS = .

include_subdirs :=

DIST_SUBDIRS := $(SUBDIRS) $(include_subdirs)

gomwidgets.c: gomwidgets.in Makefile
	cp $< $@.tmp
	awk '/GTK_TYPE_.*get_type/ { print "WIDGET ("$$2")" }' \
		$(shell pkg-config --variable includedir gtk+-2.0)/gtk-2.0/gtk/*.h \
		>> $@.tmp
	@if ! cmp $@.tmp $@ ; then		\
		echo cp $@.tmp $@;		\
		cp $@.tmp $@;			\
	fi

include examples/Makefile.inc
include include/gom/Makefile.inc
include include/gom/dom/Makefile.inc
include src/gom/Makefile.inc
include src/libgom/Makefile.inc
include src/schema/Makefile.inc
include tests/Makefile.inc

update-makefiles:
	@for dir in $(include_subdirs) ; do \
		echo " cp $(top_srcdir)/Makefile.am.subdir $(top_srcdir)/$$dir/Makefile.am" ; \
		cp "$(top_srcdir)/Makefile.am.subdir" "$(top_srcdir)/$$dir/Makefile.am" || exit 1 ; \
	done
