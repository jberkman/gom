# -*- Makefile -*-

lib_LTLIBRARIES += libgom.la

libgom_la_SOURCES :=				\
	include/gommacros.h			\
	src/libgom/gomdoc.c			\
	src/libgom/gomdom.c			\
	src/libgom/gomdominterfaces.c		\
	src/libgom/gomglist.c			\
	src/libgom/gomjscontext.c		\
	src/libgom/gomjsdocument.c		\
	src/libgom/gomjselement.c		\
	src/libgom/gomjsexception.c		\
	src/libgom/gomjsobject.c		\
	src/libgom/gomjsnavigator.c		\
	src/libgom/gomjsnode.c			\
	src/libgom/gomjswindow.c		\
	src/libgom/gomobject.c			\
	src/libgom/gomvalue.c			\
	src/libgom/gomwidget.c

libgom_la_CFLAGS = -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS)
libgom_la_LDFLAGS = $(GOM_LIBS)

src/libgom/gomwidget.c: gomwidgets.c

CLEANFILES += gomwidgets.c gomwidgets.c.tmp
