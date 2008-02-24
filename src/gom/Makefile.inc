# -*- Makefile -*-

bin_PROGRAMS += gom

gom_SOURCES := src/gom/gom.c
gom_CFLAGS = -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS)
gom_LDFLAGS = $(GOM_LIBS) $(JS_LIBS)
gom_LDADD := libgom.la
