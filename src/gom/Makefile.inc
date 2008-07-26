# -*- Makefile -*-

bin_PROGRAMS += gom

gom_SOURCES := src/gom/gom.c
gom_CFLAGS =  -I$(top_builddir)/include -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS) $(PY_CPPFLAGS)
gom_LDADD := $(PY_LDFLAGS) libgom.la $(GOM_LIBS) $(JS_LIBS) $(CURL_LIBS)
