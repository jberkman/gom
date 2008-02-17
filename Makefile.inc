# -*- Makefile -*-

bin_PROGRAMS += gom

lib_LTLIBRARIES += libgom.la

pkgconfig_DATA += gom-0.pc

domincludedir := $(includedir)/gom-0/gom/dom
dominclude_HEADERS :=				   \
	include/gom/dom/gomattr.h		   \
	include/gom/dom/gomcdatasection.h	   \
	include/gom/dom/gomcharacterdata.h	   \
	include/gom/dom/gomcomment.h		   \
	include/gom/dom/gomdocument.h		   \
	include/gom/dom/gomdocumentfragment.h	   \
	include/gom/dom/gomdocumenttype.h	   \
	include/gom/dom/gomdomexception.h	   \
	include/gom/dom/gomdomimplementation.h	   \
	include/gom/dom/gomelement.h		   \
	include/gom/dom/gomentityreference.h	   \
	include/gom/dom/gomnamednodemap.h	   \
	include/gom/dom/gomnode.h		   \
	include/gom/dom/gomnodelist.h		   \
	include/gom/dom/gomnotation.h		   \
	include/gom/dom/gomprocessinginstruction.h \
	include/gom/dom/gomtext.h

gomincludedir := $(includedir)/gom-0/gom
gominclude_HEADERS :=				\
	include/gom/gomdoc.h			\
	include/gom/gomdom.h			\
	include/gom/gomglist.h			\
	include/gom/gomjsdocument.h		\
	include/gom/gomjselement.h		\
	include/gom/gomjsobject.h		\
	include/gom/gomjsnode.h			\
	include/gom/gomjswindow.h		\
	include/gom/gomwidget.h

libgom_la_SOURCES :=				\
	include/gommacros.h			\
	src/libgom/gomdoc.c			\
	src/libgom/gomdom.c			\
	src/libgom/gomglist.c			\
	src/libgom/gominterfaces.c		\
	src/libgom/gomjsdocument.c		\
	src/libgom/gomjselement.c		\
	src/libgom/gomjsobject.c		\
	src/libgom/gomjsnode.c			\
	src/libgom/gomjswindow.c		\
	src/libgom/gomwidget.c

src/libgom/gomwidget.c: gomwidgets.c

gomwidgets.c: Makefile.inc
	awk '/GTK_TYPE_.*get_type/ { print "type ^= "$$2";" }' \
		$(shell pkg-config --variable includedir gtk+-2.0)/gtk-2.0/gtk/*.h > $@.tmp
	@if ! cmp $@.tmp $@ ; then		\
		echo cp $@.tmp $@;		\
		cp $@.tmp $@;			\
	fi

libgom_la_CFLAGS = -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS)
libgom_la_LDFLAGS = $(GOM_LIBS)

gom_SOURCES := src/gom/gom.c
gom_CFLAGS = -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS)
gom_LDFLAGS = $(GOM_LIBS) $(JS_LIBS)
gom_LDADD := libgom.la

dist_noinst_DATA += Makefile.configure

CLEANFILES += gomwidgets.c gomwidgets.c.tmp
